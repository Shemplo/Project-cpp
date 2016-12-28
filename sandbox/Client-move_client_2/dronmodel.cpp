#include "gameform.h"
#include "dronmodel.h"

DronModel::DronModel (GameForm* form, qint32 number) : QGraphicsItem () {
	width = 40, height = 20;
	bounds = QRectF (-(width + 10) / 2, 
					 -(height + 10) / 2, 
					 width + 20, 
					 height + 20);
	this->form   = form;
	this->number = number;
}

QRectF DronModel::boundingRect () const {
	return bounds;
}

void DronModel::paint (QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) {
	Q_UNUSED (option);
	Q_UNUSED (widget);
	
	QRect rectangle;
	rectangle.setX (-width / 2);
	rectangle.setY (-height / 2);
	rectangle.setWidth (width);
	rectangle.setHeight (height);
	painter->setBrush (Qt::gray);
	painter->drawRect (rectangle);
	
	QPolygon tank;
	tank << QPoint (-7, 8) 
		 << QPoint (-7, 4) 
		 << QPoint (-17, 4)
		 << QPoint (-17, 8);
	painter->setBrush (Qt::darkGray);
	painter->drawPolygon (tank);
	
	tank.clear ();
	tank << QPoint (-7, -8) 
		 << QPoint (-7, -4) 
		 << QPoint (-17, -4)
		 << QPoint (-17, -8);
	painter->setBrush (Qt::darkGray);
	painter->drawPolygon (tank);
	
	painter->rotate (form->turretAngles.at (number));
	
	QPolygon cabin;
	cabin << QPoint (-5, 5) 
		  << QPoint (-5, -5) 
		  << QPoint (10, -5) 
		  << QPoint (10, 5);
	painter->setBrush (Qt::darkGray);
	painter->drawPolygon (cabin);
	
	QLine gun;
	gun.setP1 (QPoint (20, 0));
	gun.setP2 (QPoint (10, 0));
	painter->setPen (QPen (QBrush (Qt::black), 2));
	painter->drawLine (gun);
	
	painter->rotate (0);
}
