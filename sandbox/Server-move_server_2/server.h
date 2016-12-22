#ifndef SEVER_H
#define SEVER_H

#include <iostream>

#include <QTimer>
#include <QObject>
#include <QVector>
#include <QDateTime>
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
		void joinToBattle (qint32 identif);
		void leaveQueue   (qint32 identif);
		
		qint32 queueSize  ();
		
	public slots:
		void slotNewConnection ();
		void slotPulse         ();
		
	private:
		bool working = false;
		
		QTcpServer*       server;
		QVector <Client*> connects;
		QVector <qint32>  queue;
		
		QTimer* pulse;
		
		qint32 nextId ();
		
};

#endif // SEVER_H
