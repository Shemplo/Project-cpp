#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <windows.h>

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class GameScene : public QGraphicsScene {
		
	Q_OBJECT	
		
	public:
		explicit GameScene (QObject* paretn = 0);
		
	signals:
		void signalMove  (QPointF point);
		void signalClick (int button);
		
	private:
		void mouseMoveEvent    (QGraphicsSceneMouseEvent* event);
		void mousePressEvent   (QGraphicsSceneMouseEvent* event);
		void mouseReleaseEvent (QGraphicsSceneMouseEvent* event);
		
};

#endif // GAMESCENE_H
