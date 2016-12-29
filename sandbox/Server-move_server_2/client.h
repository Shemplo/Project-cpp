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
		// Default constructor for Client
		explicit Client (QTcpSocket* socket, qint32 id, Server* server);
		
		// Public method to write into socket
		// data represented in QByteArray
		void writeInSocket (QByteArray data);
		
		// Return identificator of socket
		qint32               getId      ();
		// Return data of selected machine
		MachineInfo::Machine getMachine ();
		
		// Trigges to know the direction of dron
		bool forward = false, 
			 left    = false, 
			 right   = false;
		
		// Point where cursor is
		QPointF target;
		
		// Shots that is in queue
		QQueue <QPointF> shots;
		
	signals:
		// Send info that client entered
		// the game. And loaded all data
		void signalGameReady (qint32 id);
		
	private slots:
		// Ready to fetch data from stream
		void slotReadStream ();
		
		// Handle that connection lost
		void slotDisconnect ();
		
	private:
		// Identificator of client
		qint32 identf = 0;
		
		// Data about machine
		MachineInfo machineInfo;
		
		// List of machines in hangar
		QVector <qint32> machines;
		
		// ID of selected machine
		qint32 selectedMachine = 0;
		
		// User balance info
		qint32 credits    = 100;
		qint32 bonuses    = 0;
		qint32 experience = 0;
		
		// Server
		Server*     server;
		QTcpSocket* socket;
		
		// For reading data from stream
		qint32 need = 0;
		QByteArray bufferArray;
		
		// Parse data for commands in stream
		// and do that is defined due to target
		void parseInputStream (QByteArray data);
		
		// Send informtioin to socket about 
		// machine characteristics
		void sendMachineInfo  (qint32 id);
		
		// Send information to socket about
		// user balance (credits, exp, bons)
		void sendBalanceInfo  ();
		
};

#endif // CLIENTCONNECTION_H
