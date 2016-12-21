#ifndef SEVER_H
#define SEVER_H

#include <iostream>

#include <QTimer>
#include <QObject>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>

class Client;

class Server : public QObject {
		
	Q_OBJECT
		
	public:
		explicit Server (QObject* parent = 0);
		
		void start (qint32 port);
		void stop  (qint32 code);
		
		void removeClient (Client* client);
		
	public slots:
		void slotNewConnection ();
		void slotPulse         ();
		
	private:
		bool working = false;
		
		QTcpServer*       server;
		QVector <Client*> connects;
		
		QTimer* pulse;
		
		qint32 nextId ();
		
};

#endif // SEVER_H
