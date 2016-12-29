#ifndef GAME_H
#define GAME_H

#include <QLineF>
#include <QTimer>
#include <QVector>
#include <QObject>
#include <QGraphicsItem>

#include <server.h>
#include <mapinfo.h>

class Game : public QObject {
		
	Q_OBJECT
		
	public:
		explicit Game (Server* app);
		
		// Add new player in game from queue
		bool addPlayer   (Client* client);
		
		// Set status that adding of new
		// users from queue is clised
		void closeAdding ();
		
	public slots:
		// Clietn send information that it 
		// downloaded information from server
		// Mark that it's ready to start
		void slotClientReady (qint32 id);
		
		//void slotClientDied  ();
		
		// Call when the timer with period 1s
		// has timeout
		void slotSecondLeft  ();
		
		// Call when the game timer
		// has timeout (period ~8ms)
		void slotGameTick    ();
		
	private:
		// Reference to the 
		// server object
		Server* server;
		qint32 status = 0;
		
		// Clients that is in game
		// And marks if clients are ready
		QVector <Client*> clients;
		QVector <bool> readyMask;
		
		// Timer of 1s period
		QTimer* stopwatch;
		qint32 countdown = -1;
		qint32 minutes = 0, seconds = 0;
		
		// Flag if it's single game
		bool isSingle = false;
		
		// Method called when all
		// clients send info of ready
		void startGame ();
		
		// Game timer of ~8ms period
		QTimer* gameTimer;
		
		// Information of map in game
		MapInfo             mapInfo;
		QVector <QPolygonF> obstacles;
		
		qint32  selectedMap = 0;
		
		// Boundings of map
		qint32  awidth  = 490;
		qint32  aheight = 490;
		
		// Model of dron that used for
		// all movements and interaction
		struct Dron {
			qreal x = 0, y = 0; 
			qreal angle    = 0;
			qreal diagonal = 0;
			
			qreal health = 0, armor    = 0;
			qreal speed  = 0, rotate   = 0;
			qreal damage = 0, duration = 0;
			qreal bullet = 0, capacity = 0;
			qint32 width = 0, height   = 0;
			qreal penetration = 0;
			qreal turretAngle = 0;
			QPointF target;
			
			qint32 timeout = 0;
			
			qreal battleDamege = 0;
			qreal battleKills  = 0;
			qint32 winPoints   = 0;
		};
		
		// Model of bullet that used for
		// all movements and interaction
		struct Bullet {
			qreal x = 0, y = 0;
			qreal angle    = 0;
			
			qreal speed  = 0;
			qreal damage = 0;
			qreal penetration = 0;
			
			qint32 fromId = -1;
			
			qreal  width  = 12;
			qreal  height = 2;
		};
		
		// List of drons
		QVector <Dron*>   drons;
		
		// List of bullets
		QVector <Bullet*> bullets;
		
		// Game status (flag if game started)
		bool playing = false;
		
		// Change positions of drons
		// with checking collisions
		void moveDrons   ();
		
		// Change positions of bullets
		// with checking collisions
		void moveBullets ();
		
		// Check if model went out of the arena
		bool checkArenaBounds (QString asix, 
							   qreal x, qreal y, 
							   qreal angle, 
							   qreal diagonal,
							   qreal dalpha);
		
		// Check if model went into obstacle
		bool checkObstacles   (QString asix,
							   qreal x, qreal y,
							   qreal angle,
							   qreal width, qreal height,
							   qreal dalpha);
		
		// Check if model went into dron
		qint32 checkDrons      (QString asix,
							   qreal x, qreal y,
							   qreal angle,
							   qreal width, qreal height,
							   qreal dalpha);
		
		// Make angle in [-180, 180]* deg
		qreal normalizeAngle (qreal angle);
		
		// Translate angle from deg to rad
		qreal static rad (qreal angle);
		
		// Model that used to check all collisions
		class CollisionModel : public QGraphicsItem {
			
			public:
				void      setBounds    (QRectF bounds);
				
				// Get bounds of this model
				QRectF    boundingRect () const;
				
				// Get bounds if this model
				// considerins the angle of rotation
				QPolygonF boundingPolygon (qreal x, qreal y, 
										   qreal a, qreal dalpha);
				
				// Stub method that need by QGraphicsItem
				void paint (QPainter* painter, 
							const QStyleOptionGraphicsItem* option,
							QWidget* widget);
				
			private:
				QRectF bounds;
				
		};
		
		// Collisions models
		CollisionModel* model;
		CollisionModel* model1;
		
};

#endif // GAME_H
