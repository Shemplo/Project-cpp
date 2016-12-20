#include "dron.h"
#include "bullet.h"
#include "drawclient.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

Dron::Dron (QObject* parent) : QObject (parent),
								QGraphicsItem () {
	this->manual = false;
	width = 40, height = 20;
	awidth = 0, aheight = 0;
	shotTimeout = 0;
	
	point  = QPointF (std::rand () % 150 - 75.0, std::rand () % 100 - 50.0);
	bounds = QRectF (-(width + 10) / 2, -(height + 10) / 2, width + 20, height + 20);
	diagonal = std::sqrt (width * width + height * height) / 2;
	std::cout << "Diagonal: " << diagonal << std::endl;
	
	speed = 2;
	angle = std::rand () % 180;
}

Dron::~Dron () {}

void   Dron::id (qint32 id) { this->identificator = id; }
qint32 Dron::id ()          { return identificator; }

void Dron::setManualControl (bool manual) {
	this->manual = manual;
}

void Dron::setRectangleArea (int width, int height) {
	this->awidth  = width;
	this->aheight = height;
	
	std::cout << "Area width: "  << awidth << std::endl;
	std::cout << "Area height: " << aheight << std::endl;
}

void Dron::setDrawClient (DrawClient* client) {
	this->client = client;
}

QRectF Dron::boundingRect () const {
	return bounds;
}

qreal Dron::rad (qreal angle) {
	return angle * Pi / 180;
}

void Dron::normalizeAngle () {
	while (angle > 180) { angle -= 360; }
	while (angle < -180) { angle += 360; }
}

bool Dron::checkBoundsX (qreal x) {
	qreal dalpha = std::atan ((double) height / (double) width) / 2;
	
	if (angle >= -180 && angle < -90) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) - dalpha)) > awidth / 2) { return false; }
	} else if (angle >= -90 && angle < 0) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) + dalpha)) > awidth / 2) { return false; }
	} else if (angle >= 0 && angle < 90) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) - dalpha)) > awidth / 2) { return false; }
	} else if (angle >= 90 && angle <= 180) {
		if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) + dalpha)) > awidth / 2) { return false; }
	}
	
	return true;
}

bool Dron::checkBoundsY (qreal y) {
	qreal dalpha = std::atan ((double) height / (double) width) / 2;
	
	if (angle >= -180 && angle < -90) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) + dalpha)) > aheight / 2) { return false; }
	} else if (angle >= -90 && angle < 0) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) - dalpha)) > aheight / 2) { return false; }
	} else if (angle >= 0 && angle < 90) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) + dalpha)) > aheight / 2) { return false; }
	} else if (angle >= 90 && angle <= 180) {
		if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) - dalpha)) > aheight / 2) { return false; }
	}
	
	return true;
}

bool Dron::checkDrons (qreal x, qreal y) {
	QVector <Dron *> drons = client->getDrons ();
	QPolygonF bounds = boundingPolygon (x, y, angle);
	
	for (qint32 i = 0; i < drons.size (); i ++) {
		if (drons.at (i)->id () == identificator) { continue; }
		if (!drons.at (i)->boundingPolygon ().intersected (bounds).isEmpty ()) { return false; }
	}
	
	return true;
}

bool Dron::checkObstacles (qreal x, qreal y) {
	QVector <QPolygonF> obstacles = client->getObstacles ();
	QPolygonF bounds = boundingPolygon (x, y, angle);
	
	for (qint32 i = 0; i < obstacles.size (); i ++) {
		//for (qint32 j = 0; j < bounds.size (); j ++) {
		//	if (obstacles.at (i).containsPoint (bounds.at (j), Qt::WindingFill)) { return false; }
		//}
		if (!obstacles.at (i).intersected (bounds).isEmpty ()) { return false; }
	}
	
	return true;
}

QPolygonF Dron::boundingPolygon () {
	return boundingPolygon (point.x (), point.y (), angle);
}

QPolygonF Dron::boundingPolygon (qreal x, qreal y, qreal a) {
	qreal dalpha = ::atan ((double) height / (double) width) / 2;
	
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

void Dron::paint (QPainter* painter,
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
	
	if (manual) {
		QLineF toTarget (QPointF (point.x (), point.y ()), target);
		qreal angleToTarget = -toTarget.angle ();
		
		if (std::abs (angleToTarget - angle) >= 0) { painter->rotate (angleToTarget - angle); } 
		else                                       { painter->rotate (angle - angleToTarget); }
		
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
	
	/*QPolygonF bounds = boundingPolygon ();
	painter->setBrush (Qt::green);
	
	for (qint32 i = 0; i < bounds.size (); i ++) {
		//qreal x = bounds.at (i).x (), y = bounds.at (i).y ();
		painter->drawEllipse (bounds.at (i), 2, 2);
	}*/
}

void Dron::slotMove () {
	//qreal dalpha = ::atan ((double) height / (double) width) / 2;
	qreal c  = std::cos (angle * Pi / 180), s  = std::sin (angle * Pi / 180);
	
	if (manual) {
		speed = 2;
		qreal x = point.x (), y = point.y ();
		
		if (client->isActiveButton (Qt::Key_W)) {
			x = point.x () + c * speed;
			y = point.y () + s * speed;
			
			if (!checkDrons (x, y)) { x -= c * speed; y -= s * speed; }
			if (!checkObstacles (x, y)) { x -= c * speed; y -= s * speed; }
		} else if (client->isActiveButton (Qt::Key_S)) {
			x = point.x () - c * speed;
			y = point.y () - s * speed;
			
			//if (!checkDrons (x, y)) { x += c * speed; y += s * speed; }
			if (!checkObstacles (x, y)) { x += c * speed; y += s * speed; }
		}
		
		if (!checkBoundsX (x)) { x -= c * speed; }
		if (!checkBoundsY (y)) { y -= s * speed; }
		
		point.setX (x);
		point.setY (y);
		
		if (client->isActiveButton (Qt::Key_A) || client->isActiveButton (Qt::Key_D)) {
			qreal x = point.x ();
			qreal y = point.y ();
			qreal prev = angle;
			
			if (client->isActiveButton (Qt::Key_A)) { angle -= 2; }
			else                                    { angle += 2; }
			if (angle > 180 || angle < -180) { normalizeAngle (); }
			
			if (!checkBoundsX (x)) { angle = prev; }
			if (!checkBoundsY (y)) { angle = prev; }
		}
	} else {
		qreal x = point.x () + c * speed;
		qreal y = point.y () + s * speed;
		
		if (!checkObstacles (x, y + s * speed)) { x -= c * speed; angle += 9; }
		if (!checkObstacles (x + c * speed, y)) { y -= s * speed; angle += 7; }
		
		if (!checkDrons (x, y + s * speed)) { x -= c * speed; angle += 9; }
		if (!checkDrons (x + c * speed, y)) { y -= s * speed; angle += 7; }
		
		if (!checkBoundsX (x)) { x -= c * speed; angle += 69; }
		if (!checkBoundsY (y)) { y -= s * speed; angle += 67; }
		if (angle > 180 || angle < -180) { normalizeAngle (); }
		
		//std::cout << "X: " << x << " " << diagX * c << std::endl;
		//std::cout << "Y: " << y << " " << diagY * s << std::endl;
		//std::cout << ::sin (rad (angle) - dalpha) << std::endl;
		point.setX (x);
		point.setY (y);
	}
	
	this->setPos (point);
	setRotation (angle);
	//std::cout << "Tick!" << std::endl;
	
	//Special update
	shotTimeout += shotTimeout < 128 ? 1 : 0;
}

void Dron::slotTarget (QPointF target) {
	this->target = target;
}

void Dron::slotShot () {
	if (shotTimeout >= 32) {
		QLineF toTarget (QPointF (point.x (), point.y ()), target);
		qreal angleToTarget = - toTarget.angle ();
		
		Bullet* bullet = new Bullet (point, angleToTarget, this);
		bullet->setDrawClient (client);
		bullet->id (identificator);
		
		client->makeShot (bullet);
		shotTimeout = 0;
	}
}
