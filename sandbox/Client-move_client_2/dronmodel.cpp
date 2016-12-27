#include "dronmodel.h"

DronModel::DronModel () : QGraphicsItem () {
	width = 40, height = 20;
	bounds = QRectF (-(width + 10) / 2, 
					 -(height + 10) / 2, 
					 width + 20, 
					 height + 20);
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
}
