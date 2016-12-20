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
		
	public slots:
		void slotMove ();
		
	protected:
		QRectF boundingRect () const;
		void paint (QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget);
		
	private:
		qint32	width, height;
		qreal   diagonal;
		qreal   speed;
		
		QRectF  bounds;
		QPointF point;
		qreal   angle;
		
		qreal rad            (qreal angle);
		bool  checkBoundsX   (qreal x);
		bool  checkBoundsY   (qreal y);
		
		DrawClient* client;
		
};

#endif // BULLET_H
