#include <QCoreApplication>
#include "moveserver.h"
#include "serverconnection.h"
#include <iostream>

MoveServer::MoveServer (QObject* parent) : QObject (parent) {
	areaWidth  = 300;
	areaHeight = 175;
	
	server = new QTcpServer (this);
	bool started = server->listen (QHostAddress::Any, 16310);
	
	if (started) { std::cout << "Server started" << std::endl; }
	else         { std::cout << "Server not started" << std::endl; }
	
	connect (server, &QTcpServer::newConnection, 
			 this, &MoveServer::slotNewConnection);
	
	QTimer* timer = new QTimer (this);
	timer->setInterval (1000 / 30);
	timer->start ();
	
	connect (timer, &QTimer::timeout,
			 this, &MoveServer::slotUpdate);
}

qint32 MoveServer::activeSockets () {
	return connects.size ();
}

QVector <std::pair <qint32, qint32>>* MoveServer::positions () {
	return coords;
}

void MoveServer::disconnectSocket (ServerConnection* socket) {
	std::cout << "Disconnecting..." << std::endl;
	qint32 index = connects.indexOf (socket, 0);
	connects.remove (index);
	
	std::cout << "Socket " << index << " disconnected!" << std::endl;
}

void MoveServer::slotNewConnection () {
	std::cout << "New connection :)" << std::endl;
	QTcpSocket* socket = server->nextPendingConnection ();
	ServerConnection* connection = new ServerConnection (socket, this);
	
	connect (this, &MoveServer::signalUpdate, 
			 connection, &ServerConnection::slotServerUpdate);
	connects.push_back (connection);
	std::cout << "Total number of connections: " << connects.size () << std::endl;
}

void MoveServer::slotUpdate () {
	coords = new QVector <std::pair <qint32, qint32>> ();
			
	for (qint32 i = 0; i < connects.size (); i ++) {
		ServerConnection* connection = connects [i];
		
		connection->x += connection->vx;
		if (connection->x < 0) {
			connection->x = 0;
			connection->vx = -connection->vx;
		}
		
		if (connection->x + connection->sx > this->areaWidth) {
			connection->x = this->areaWidth - connection->sx;
			connection->vx = -connection->vx;
		}
		
		connection->y += connection->vy;
		if (connection->y < 0) {
			connection->y = 0;
			connection->vy = -connection->vy;
		}
		
		if (connection->y + connection->sy > this->areaHeight) {
			connection->y = this->areaHeight - connection->sy;
			connection->vy = -connection->vy;
		}
		
		coords->push_back (std::make_pair (connection->x, connection->y));
	}
	
	emit signalUpdate ();
}
