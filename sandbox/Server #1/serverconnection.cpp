#include "serverconnection.h"
#include "moveserver.h"
#include <iostream>

ServerConnection::ServerConnection (QTcpSocket* socket, 
									MoveServer* server)
	: QObject (server) {
	this->socket = socket;
	this->server = server;
	
	x  = y  = 15;
	vx = vy = 1;
	sx = sy = 10;
	
	connect (this->socket, &QTcpSocket::readyRead,
			 this, &ServerConnection::slotReadStream);
	connect (this->socket, &QTcpSocket::disconnected,
			 this, &ServerConnection::slotDropConnection);
}

void ServerConnection::slotReadStream () {
	while (socket->bytesAvailable () > 0) {
		QByteArray value = socket->readLine ();
		
		QDataStream input (&value, QIODevice::ReadOnly);
		QString command; input >> command;
		
		if (command == "speed") {
			int vx = 0, vy = 0; 
			input >> vx >> vy;
			
			std::cout << "New speed: " << vx << " " << vy << std::endl;
		}
	}
}

void ServerConnection::slotServerUpdate () {
	QByteArray value;
	QDataStream output (&value, QIODevice::WriteOnly);
	
	QString command = "squares";
	qint32  number  = server->activeSockets ();
	
	output << command;
	output << number;
	
	QVector <std::pair <qint32, qint32>>* coords = server->positions ();
	for (int i = 0; i < number; i ++) {
		output << coords->at(i).first;
		output << coords->at(i).second;
	}
	
	output << "\n";
	socket->write (value);
}

void ServerConnection::slotDropConnection () {
	std::cout << "Dropping connection" << std::endl;
	server->disconnectSocket (this);
	socket->close ();
	delete this;
}
