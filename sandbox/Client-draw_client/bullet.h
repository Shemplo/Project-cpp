#ifndef BULLET_H
#define BULLET_H

#include <iostream>

#include <QObject>
#include <QGraphicsItem>

#include <drawclient.h>

class Bullet : public QObject, public QGraphicsItem {
		
	Q_OBJECT
		
	public:
		Bullet (QPointF start, qreal angle, QObject* parent = 0);
		void setDrawClient    (DrawClient* client);
		void id (qint32 id);
		
	public slots:
		void slotMove ();
		
	protected:
		QRectF boundingRect () const;
		void paint (QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget);
		
	private:
		qint32  identificator;
		qint32	width, height;
		qreal   diagonal;
		qreal   speed;
		
		QRectF  bounds;
		QPointF point;
		qreal   angle;
		
		qreal  rad            (qreal angle);
		bool   checkBoundsX   (qreal x);
		bool   checkBoundsY   (qreal y);
		qint32 checkDrons     (qreal x, qreal y);
		bool   checkObstacles (qreal x, qreal y);
		
		QPolygonF boundingPolygon ();
		QPolygonF boundingPolygon (qreal x, qreal y, qreal a);
		
		DrawClient* client;
		
};

#endif // BULLET_H
