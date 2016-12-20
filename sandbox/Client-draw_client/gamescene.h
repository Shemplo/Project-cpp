#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <iostream>

#include <QObject>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class GameScene : public QGraphicsScene {
		
	Q_OBJECT	
		
	public:
		explicit GameScene (QObject* paretn = 0);
		
	signals:
		void signalMove   (QPointF point);
		void signalClick  (int button);	
		void signalButton (int button, int action);
		
	private:
		void mouseMoveEvent    (QGraphicsSceneMouseEvent* event);
		void mousePressEvent   (QGraphicsSceneMouseEvent* event);
		void mouseReleaseEvent (QGraphicsSceneMouseEvent* event);
		
		void keyPressEvent     (QKeyEvent* event);
		void keyReleaseEvent   (QKeyEvent* event);
		
		bool* buffer = new bool [512];
		
};

#endif // GAMESCENE_H
