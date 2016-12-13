#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class MoveServer;

class ServerConnection : public QObject
{
	Q_OBJECT
		
	public:
		explicit ServerConnection (QTcpSocket* socket, 
								   MoveServer* server);
		qint32 x, y, vx, vy, sx, sy;
		
	public slots:
		void slotReadStream ();
		void slotServerUpdate ();
		void slotDropConnection ();
		
	private:
		MoveServer* server;
		QTcpSocket* socket;
		
};

#endif // SERVERCONNECTION_H
