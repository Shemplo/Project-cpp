#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <iostream>

#include <QQueue>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

#include <machineinfo.h>

class Server;

class Client : public QObject {
		
	Q_OBJECT
		
	public:
		explicit Client (QTcpSocket* socket, qint32 id, Server* server);
		
		void writeInSocket (QByteArray data);
		
		qint32               getId      ();
		MachineInfo::Machine getMachine ();
		
		bool forward = false, 
			 left    = false, 
			 right   = false;
		
		QPointF target;
		
		QQueue <QPointF> shots;
		
	signals:
		void signalGameReady (qint32 id);
		
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
		
		MachineInfo machineInfo;
		QVector <qint32> machines;
		qint32 selectedMachine = 0;
		
		qint32 credits    = 100;
		qint32 bonuses    = 0;
		qint32 experience = 0;
		
		Server*     server;
		QTcpSocket* socket;
		
		qint32 need = 0;
		QByteArray bufferArray;
		
		void parseInputStream (QByteArray data);
		void sendMachineInfo  (qint32 id);
		void sendBalanceInfo  ();
		
};

#endif // CLIENTCONNECTION_H
