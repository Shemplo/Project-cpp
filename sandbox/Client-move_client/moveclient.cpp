#include "moveclient.h"
#include <iostream>

MoveClient::MoveClient (QObject* parent) : QObject (parent) {
	socket = new QTcpSocket (this);
	//socket->connectToHost ("192.169.0.1", 16310);
	socket->connectToHost ("127.0.0.1", 16310);
	socket->waitForConnected ();
	
	connect (socket, &QTcpSocket::readyRead,
			 this, &MoveClient::slotReadStream);
	connect (socket, &QTcpSocket::disconnected,
			 this, &MoveClient::slotDisconnected);
	
	std::cout << "Connected to server!" << std::endl;
}

void MoveClient::slotReadStream () {
	while (socket->bytesAvailable () > 0) {
		QByteArray value = socket->readLine ();
		
		QDataStream input (&value, QIODevice::ReadOnly);
		QString command; input >> command;
		
		if (command == "squares") {
			qint32 number = 0;
			input >> number;
			
			std::cout << number << std::endl;
			while (number > 0) {
				qint32 x; input >> x;
				qint32 y; input >> y;
				
				std::cout << x << " " << y << std::endl;
				number --;
			}
		}
	}
}

void MoveClient::slotDisconnected () {
	std::cout << "Disconnected from server!" << std::endl;
	socket->close ();
}
