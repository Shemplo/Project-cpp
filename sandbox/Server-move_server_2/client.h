#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

class Server;

class Client : public QObject {
		
	Q_OBJECT
		
	public:
		explicit Client (QTcpSocket* socket, qint32 id, Server* server);
		
		qint32 getId ();
		
	private slots:
		void slotReadStream ();
		void slotDisconnect ();
		
	private:
		/*
		Status list:
		0 - active, not in game
		1 - active, in queue
		2 - active, in game
		*/
		qint32 status = 0;
		qint32 identf = 0;
		
		qint32 selectedMachine = 0;
		
		Server*     server;
		QTcpSocket* socket;
		
		qint32 need = 0;
		QByteArray bufferArray;
		
		void parseInputStream (QByteArray data);
		
};

#endif // CLIENTCONNECTION_H
