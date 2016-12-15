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
		bufferArray.append (socket->readAll ());
		std::cout << bufferArray.size () << std::endl;
		
		if ((bufferArray.size () >= sizeof (qint32)) && need == 0) {
			QDataStream input (&bufferArray, QIODevice::ReadOnly);
			input >> need;
			
			std::cout << need << std::endl;
			bufferArray.remove (0, sizeof (qint32));
		}
		
		if (bufferArray.size () >= need && need != 0) {
			QDataStream input (&bufferArray, QIODevice::ReadOnly);
			QString command; input >> command;
			qint32  number;  input >> number;
			std::cout << "Command: " << command.toStdString () << std::endl;
			std::cout << "Clients: " << number << std::endl;
			
			for (int i = 0; i < number; i ++) {
				qint32 x, y; input >> x >> y;
				std::cout << x << " " << y << std::endl;
			}
			
			bufferArray.remove (0, need);
			need = 0;
		}
	}
}

void MoveClient::slotDisconnected () {
	std::cout << "Disconnected from server!" << std::endl;
	socket->close ();
}
