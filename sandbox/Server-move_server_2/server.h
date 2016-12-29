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
class Game;

class Server : public QObject {
		
	Q_OBJECT
		
	public:
		explicit Server (QObject* parent = 0);
		
		// Start server on given port
		void start (qint32 port);
		
		// Stop server with  stopping code
		void stop  (qint32 code);
		
		// Remove client from active sockets
		// due to it (socket) die
		void removeClient (Client* client);
		
		// Move client socket to game class
		void joinToBattle (Client* client);
		
		// Remove client socket from game
		// queue (button leave pushed)
		void leaveQueue   (Client* client);
		
		// Remove game from list of games
		void closeGame (Game* game);
		
		// Get number of clients in queue
		qint32 queueSize  ();
		
	public slots:
		// Called when new connection
		// applied (pending connection)
		void slotNewConnection ();
		
		// Called when timer with period 10s
		// has timeout (to write info about server)
		void slotPulse         ();
		
	private:
		// Flag if server works;
		bool working = false;
		
		// Server
		QTcpServer*       server;
		
		// List of clients
		QVector <Client*> connects;
		
		// List of clients in queue
		QVector <Client*>  queue;
		
		// List of games
		QVector <Game*>   games;
		
		// Timer with period 10s
		QTimer* pulse;
		
		// Generate next unused id
		qint32 nextId ();
		
};

#endif // SEVER_H
