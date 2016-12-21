#include "server.h"
#include "client.h"

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
	qint32 index = connects.indexOf (client);
	if (index != -1) { connects.remove (index); }
	
	std::cout << "[CLIENT] Client #" << client->getId () 
			  << " disconnected from server" << std::endl;
}

qint32 Server::nextId () {
	bool ready = false;
	qint32 id  = 0;
	
	while (!ready) {
		//Generated id
		id = 0;
		
		for (int i = 0; i < 8; i ++) {
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
			  << "         Active connections: " << connects.size () << std::endl
			  << "         Server listening port: " << server->serverPort () << std::endl;
}
