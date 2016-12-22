#include "server.h"
#include "client.h"
#include "game.h"

Server::Server (QObject* parent) : QObject (parent) {
	server = new QTcpServer (this);
}

void Server::start (qint32 port) {
	bool started = server->listen (QHostAddress::Any, port);
	
	if (started) {
		std::cout << "[SERVER] Server started on port: " 
				  << port
				  << std::endl;
		
		connect (server, &QTcpServer::newConnection,
				 this, &Server::slotNewConnection);
		
		pulse = new QTimer (this);
		pulse->start (10000);
		
		connect (pulse, &QTimer::timeout,
				 this, &Server::slotPulse);
		
		working = true;
	} else {
		std::cout << "[SERVER] Failed to start server :(" 
				  << std::endl;
		server->close ();
	}
}

void Server::stop (qint32 code) {
	if (this->working) {
		std::cout << "[SERVER] Server stopped with code: " 
				  << code 
				  << std::endl;
		
		server->close ();
	}
}

void Server::removeClient (Client* client) {
	std::cout << "[CLIENT] Disconnecting #" 
			  << client->getId () << " ..." << std::endl;
	leaveQueue (client);
	
	qint32 index = connects.indexOf (client);
	if (index != -1) { connects.remove (index); }
	
	std::cout << "[CLIENT] Client #" << client->getId () 
			  << " disconnected from server" << std::endl;
}

void Server::joinToBattle (Client* client) {
	qint32 index = queue.indexOf (client);
	if (index != -1) { return; }
	
	for (qint32 i = 0; i < queue.size (); i ++) {
		QByteArray write;
		QDataStream output (&write, QIODevice::WriteOnly);
		QString command = "queue"; output << command;
		QString target  = "users"; output << target;
		
		//Writing how many players in queue
		output << queueSize () + 1;
		
		queue.at (i)->writeInSocket (write);
	}
	
	queue.append (client);
	
	if (queueSize () >= 2) {
		Game* game = new Game (this);
		bool added = true;
		
		added = added && game->addPlayer (queue.at (0));
		added = added && game->addPlayer (queue.at (1));
		
		if (added) {
			queue.removeFirst ();
			queue.removeFirst ();
			
			games.append  (game);
			game->closeAdding ();
		}
	}
}

void Server::leaveQueue (Client* client) {
	qint32 index = queue.indexOf (client);
	if (index != -1) { queue.remove (index); }
	
	for (qint32 i = 0; i < queue.size (); i ++) {
		QByteArray write;
		QDataStream output (&write, QIODevice::WriteOnly);
		QString command = "queue"; output << command;
		QString target  = "users"; output << target;
		
		//Writing how many players in queue
		output << queueSize ();
		
		queue.at (i)->writeInSocket (write);
	}
}

void Server::closeGame (Game* game) {
	qint32 index = games.indexOf (game);
	if (index != -1) { games.remove (index); }
}

qint32 Server::queueSize () {
	return queue.size ();
}

qint32 Server::nextId () {
	bool ready = false;
	qint32 id  = 0;
	
	while (!ready) {
		//Generated id
		id = 0;
		
		for (int i = 0; i < 6; i ++) {
			id = id * 10 + (std::rand () % 9 + 1);
		}
		
		bool flag = true;
		for (qint32 i = 0; i < connects.size (); i ++) {
			if (connects.at (i)->getId () == id) {
				flag = false;
				break;
			}
		}
		
		ready = flag;
	}
	
	return id;
}

void Server::slotNewConnection () {
	while (server->hasPendingConnections ()) {
		std::cout << "[CLIENT] New upcomming connection" << std::endl;
		QTcpSocket* socket = server->nextPendingConnection ();
		Client*     client = new Client (socket, nextId (), this);
		
		connects.push_back (client);
		std::cout << "[CLIENT] Connection applied with id #" 
				  << client->getId () << std::endl;
	}
}

void Server::slotPulse () {
	std::cout << "[INFO] Server information: " << std::endl 
			  << "         Active connections:    " << connects.size () << std::endl 
			  << "         Players in queue:      " << queue.size () << std::endl 
			  << "         Active games:          " << games.size () << std::endl
			  << "         Server listening port: " << server->serverPort () << std::endl
			  << "         Server date and time:  " << QDateTime::currentDateTime ().
														toString ("dd.MM.yyyy hh:mm:ss").
														toStdString () << std::endl;
}
