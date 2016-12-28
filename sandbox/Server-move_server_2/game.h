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
		
		bool addPlayer   (Client* client);
		void closeAdding ();
		
	public slots:
		void slotClientReady (qint32 id);
		//void slotClientDied  ();
		void slotSecondLeft  ();
		void slotGameTick    ();
		
	private:
		Server* server;
		qint32 status = 0;
		
		QVector <Client*> clients;
		QVector <bool> readyMask;
		
		QTimer* stopwatch;
		qint32 countdown = -1;
		qint32 minutes = 0, seconds = 0;
		bool isSingle = false;
		
		void startGame ();
		QTimer* gameTimer;
		
		MapInfo             mapInfo;
		QVector <QPolygonF> obstacles;
		
		qint32  selectedMap = 0;
		qint32  awidth  = 490;
		qint32  aheight = 490;
		
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
		};
		
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
		
		QVector <Dron*>   drons;
		QVector <Bullet*> bullets;
		bool playing = false;
		
		void moveDrons   ();
		void moveBullets ();
		bool checkArenaBounds (QString asix, 
							   qreal x, qreal y, 
							   qreal angle, 
							   qreal diagonal,
							   qreal dalpha);
		bool checkObstacles   (QString asix,
							   qreal x, qreal y,
							   qreal angle,
							   qreal width, qreal height,
							   qreal dalpha);
		qint32 checkDrons      (QString asix,
							   qreal x, qreal y,
							   qreal angle,
							   qreal width, qreal height,
							   qreal dalpha);
		qreal normalizeAngle (qreal angle);
		qreal static rad (qreal angle);
		
		class CollisionModel : public QGraphicsItem {
			
			public:
				void setBounds (QRectF bounds) {
					this->bounds = bounds;
				}
				
				QRectF boundingRect () const {
					return bounds;
				}
				
				QPolygonF boundingPolygon (qreal x, 
										   qreal y, 
										   qreal a,
										   qreal dalpha) {
					qreal width  = this->bounds.width ();
					qreal height = this->bounds.height ();
					qreal diagonal = std::sqrt (width * width + height * height) / 2;
					
					QPolygonF bounds;
					bounds << QPointF (x + diagonal * std::cos (rad (a) + dalpha), 
									   y - diagonal * std::sin (rad (a) + dalpha));
					bounds << QPointF (x + diagonal * std::cos (rad (a) - dalpha), 
									   y - diagonal * std::sin (rad (a) - dalpha));
					bounds << QPointF (x - diagonal * std::cos (rad (a) + dalpha), 
									   y + diagonal * std::sin (rad (a) + dalpha));
					bounds << QPointF (x - diagonal * std::cos (rad (a) - dalpha), 
									   y + diagonal * std::sin (rad (a) - dalpha));
					
					return bounds;
				}
				
				void paint (QPainter* painter, 
							const QStyleOptionGraphicsItem* option,
							QWidget* widget) {
					Q_UNUSED (painter);
					Q_UNUSED (option);
					Q_UNUSED (widget);
					// Stub
				}
				
			private:
				QRectF bounds;
				
		};
		
		CollisionModel* model;
		CollisionModel* model1;
		
};

#endif // GAME_H
