//#include "dron.h"
#include "bullet.h"
#include "drawclient.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

Bullet::Bullet (QPointF start, qreal angle, QObject* parent) : QObject (parent), 
																QGraphicsItem () {
	this->point = start;
	this->angle = angle;
	
	width = 12, height = 2;
	speed = 7;
	
	bounds = QRectF (-width / 2, -height / 2, width, height);
	diagonal = std::sqrt (width * width + height * height) / 2;
}

void Bullet::setDrawClient (DrawClient* client) {
	this->client = client;
}

QRectF Bullet::boundingRect () const {
	return bounds;
}

void Bullet::paint (QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget) {
	Q_UNUSED (option);
	Q_UNUSED (widget);
	
	QRect rectangle;
	rectangle.setX (-width / 2);
	rectangle.setY (-height / 2);
	rectangle.setWidth (width);
	rectangle.setHeight (height);
	painter->setBrush (Qt::red);
	painter->drawRect (rectangle);
}

qreal Bullet::rad (qreal angle) {
	return angle * Pi / 180;
}

bool Bullet::checkBoundsX (qreal x) {
	qreal dalpha = std::atan ((double) height / (double) width) / 2;
	
	if (angle >= -180 && angle < -90) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) - dalpha)) > client->width () / 2) { return false; }
	} else if (angle >= -90 && angle < 0) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) + dalpha)) > client->width () / 2) { return false; }
	} else if (angle >= 0 && angle < 90) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) - dalpha)) > client->width () / 2) { return false; }
	} else if (angle >= 90 && angle <= 180) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) + dalpha)) > client->width () / 2) { return false; }
	}
	
	return true;
}

bool Bullet::checkBoundsY (qreal y) {
	qreal dalpha = std::atan ((double) height / (double) width) / 2;
	
	if (angle >= -180 && angle < -90) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) + dalpha)) > client->height () / 2) { return false; }
	} else if (angle >= -90 && angle < 0) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) - dalpha)) > client->height () / 2) { return false; }
	} else if (angle >= 0 && angle < 90) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) + dalpha)) > client->height () / 2) { return false; }
	} else if (angle >= 90 && angle <= 180) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) - dalpha)) > client->height () / 2) { return false; }
	}
	
	return true;
}

void Bullet::slotMove () {
	qreal c  = std::cos (angle * Pi / 180), s  = std::sin (angle * Pi / 180);
	qreal x = point.x () + c * speed;
	qreal y = point.y () + s * speed;
	
	if (!checkBoundsX (x)) { deleteLater (); }
	if (!checkBoundsY (y)) { deleteLater (); }
	
	point.setX (x);
	point.setY (y);
	
	this->setPos (point);
	setRotation (angle);
}
