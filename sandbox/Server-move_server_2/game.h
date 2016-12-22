#ifndef GAME_H
#define GAME_H

#include <QTimer>
#include <QVector>
#include <QObject>

#include <server.h>

class Game : public QObject {
		
	Q_OBJECT
		
	public:
		explicit Game (Server* app);
		
		bool addPlayer   (Client* client);
		void closeAdding ();
		
	public slots:
		void slotClientReady (qint32 id);
		void slotSecondLeft  ();
		
	private:
		Server* server;
		qint32 status = 0;
		
		QVector <Client*> clients;
		QVector <bool> readyMask;
		
		QTimer* stopwatch;
		qint32 countdoun = 0;
		bool isSingle = false;
		
		void startGame ();
		
};

#endif // GAME_H
