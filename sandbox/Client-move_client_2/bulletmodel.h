#ifndef BULLETMODEL_H
#define BULLETMODEL_H

#include <iostream>

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>

class GameForm;

class BulletModel : public QObject, public QGraphicsItem {
		
	Q_OBJECT
		
	public:
		explicit BulletModel (GameForm*, qint32);
		
	protected:
		QRectF boundingRect () const;
		void paint (QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget);
		
	private:
		qint32 width, height;
		qint32 number;
		
		QRectF    bounds;
		GameForm* form;
		
};

#endif // BULLETMODEL_H
