#include "client.h"
#include "server.h"

Client::Client (QTcpSocket* socket, qint32 id, Server* server) : QObject (server) {
	this->status = 0;      //Just connected to server
	this->identf = id;
	this->server = server;
	this->socket = socket;
	
	connect (this->socket, &QTcpSocket::readyRead,
			 this, &Client::slotReadStream);
	connect (this->socket, &QTcpSocket::disconnected,
			 this, &Client::slotDisconnect);
}

qint32 Client::getId () {
	return this->identf;
}

void Client::slotReadStream () {
	while (socket->bytesAvailable () > 0) {
		bufferArray.append (socket->readAll ());
		
		if (bufferArray.size () >= sizeof (qint32) && need == 0) {
			QDataStream input (&bufferArray, QIODevice::ReadOnly);
			/*Reading size of input data line*/ input >> need;
			bufferArray.remove (0, sizeof (qint32));
		}
		
		if (bufferArray.size () >= need && need != 0) {
			parseInputStream (bufferArray.left (need));
			bufferArray.remove (0, need);
			need = 0;
		}
	}
}

void Client::parseInputStream (QByteArray data) {
	QDataStream input (&data, QIODevice::ReadOnly);
	QString command; input >> command;
	std::cout << "[SOCKET] Received command: " 
			  << command.toStdString () 
			  << std::endl;
	
	if (command == "ping") {
		QByteArray pong, length;
		QDataStream output (&pong, QIODevice::WriteOnly);
		output << QString ("pong");
		
		QDataStream lenout (&length, QIODevice::WriteOnly);
		lenout << pong.size ();
		
		socket->write (length);
		socket->write (pong);
	} else if (command == "selected") {
		QString target; input >> target;
		
		if (target == "machine") {
			std::cout << "[CLIENT] Sending data of selected machine number " 
					  << this->selectedMachine << std::endl;
			
			QByteArray pong, length;
			QDataStream output (&pong, QIODevice::WriteOnly);
			output << QString ("selected");
			output << QString ("machine");
			output << QString ("MS-1");
			output << (qreal) 200.0; //Health
			output << (qreal) 24.0;  //Armor
			
			QDataStream lenout (&length, QIODevice::WriteOnly);
			lenout << pong.size ();
			
			socket->write (length);
			socket->write (pong);
		}
	}
}

void Client::slotDisconnect () {
	server->removeClient (this);
	socket->close ();
	delete this;
}
