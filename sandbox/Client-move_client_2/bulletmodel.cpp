#include "gameform.h"
#include "bulletmodel.h"

BulletModel::BulletModel (GameForm* form, qint32 number) : QGraphicsItem () {
	width = 12, height = 4;
	bounds = QRectF (-(width + 10) / 2, 
					 -(height + 10) / 2, 
					 width + 20, 
					 height + 20);
	this->form   = form;
	this->number = number;
}

QRectF BulletModel::boundingRect () const {
	return bounds;
}

void BulletModel::paint (QPainter* painter,
						 const QStyleOptionGraphicsItem* option,
						 QWidget* widget) {
	Q_UNUSED (option);
	Q_UNUSED (widget);
	
	if (form->bullets.at (number)->pos () != QPointF (-100000, -100000)) {
		//painter->rotate (form->bulletAngles.at (number));
		//std::cout << form->bulletCoords.at (number).x () << std::endl;
		//this->setPos (form->bulletCoords.at (number));
		
		QRect rectangle;
		rectangle.setX (-width / 2);
		rectangle.setY (-height / 2);
		rectangle.setWidth (width);
		rectangle.setHeight (height);
		painter->setBrush (Qt::red);
		painter->drawRect (rectangle);
		
		painter->rotate (0);
	}
}
