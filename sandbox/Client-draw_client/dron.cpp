#include "dron.h"
#include "drawclient.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

Dron::Dron (QObject* parent) : QObject (parent),
								QGraphicsItem () {
	this->manual = false;
	point  = QPointF (0, 0);
	
	width = 40, height = 20;
	awidth = 0, aheight = 0;
	
	bounds = QRectF   (-width / 2, -height / 2, width, height);
	diagonal = ::sqrt (width * width + height * height) / 2;
	std::cout << "Diagonal: " << diagonal << std::endl;
	
	speed = 3;
	angle = 30;
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
	qreal dalpha = ::atan ((double) height / (double) width) / 2;
	
	if (angle >= -180 && angle < -90) {
		if (::abs (x + (diagonal + 1) * ::cos (rad (angle) - dalpha)) > awidth / 2) { return false; }
	} else if (angle >= -90 && angle < 0) {
		if (::abs (x + (diagonal + 1) * ::cos (rad (angle) + dalpha)) > awidth / 2) { return false; }
	} else if (angle >= 0 && angle < 90) {
		if (::abs (x + (diagonal + 1) * ::cos (rad (angle) - dalpha)) > awidth / 2) { return false; }
	} else if (angle >= 90 && angle <= 180) {
		if (::abs (x + (diagonal + 1) * ::cos (rad (angle) + dalpha)) > awidth / 2) { return false; }
	}
	
	return true;
}

bool Dron::checkBoundsY (qreal y) {
	qreal dalpha = ::atan ((double) height / (double) width) / 2;
	
	if (angle >= -180 && angle < -90) {
		if (::abs (y + (diagonal + 1) * ::sin (rad (angle) + dalpha)) > aheight / 2) { return false; }
	} else if (angle >= -90 && angle < 0) {
		if (::abs (y + (diagonal + 1) * ::sin (rad (angle) - dalpha)) > aheight / 2) { return false; }
	} else if (angle >= 0 && angle < 90) {
		if (::abs (y + (diagonal + 1) * ::sin (rad (angle) + dalpha)) > aheight / 2) { return false; }
	} else if (angle >= 90 && angle <= 180) {
		if (::abs (y + (diagonal + 1) * ::sin (rad (angle) - dalpha)) > aheight / 2) { return false; }
	}
	
	return true;
}

bool Dron::checkDrons (qreal x, qreal y) {
	/*QVector <Dron *> drons = client->getDrons ();
	QPolygonF bounds = boundingPolygon (x, y, angle);
	
	for (qint32 i = 0; i < drons.size (); i ++) {
		if (drons.at (i)->id () == identificator) { continue; }
		if (!drons.at (i)->boundingPolygon ().intersected (bounds).isEmpty ()) { return false; }
	}*/
	
	return true;
}

bool Dron::checkObstacles (qreal x, qreal y) {
	QVector <QPolygonF> obstacles = client->getObstacles ();
	QPolygonF bounds = boundingPolygon (x, y, angle);
	
	for (qint32 i = 0; i < obstacles.size (); i ++) {
		/*for (qint32 j = 0; j < bounds.size (); j ++) {
			if (obstacles.at (i).containsPoint (bounds.at (j), Qt::WindingFill)) { return false; }
		}*/
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
	bounds << QPointF (x + diagonal * ::cos (rad (a) + dalpha), 
					   y - diagonal * ::sin (rad (a) + dalpha));
	bounds << QPointF (x + diagonal * ::cos (rad (a) - dalpha), 
					   y - diagonal * ::sin (rad (a) - dalpha));
	bounds << QPointF (x - diagonal * ::cos (rad (a) + dalpha), 
					   y + diagonal * ::sin (rad (a) + dalpha));
	bounds << QPointF (x - diagonal * ::cos (rad (a) - dalpha), 
					   y + diagonal * ::sin (rad (a) - dalpha));
	
	if (identificator == 0) {
		for (qint32 i = 0; i < bounds.size (); i ++) {
			//qreal x = bounds.at (i).x (), y = bounds.at (i).y ();
			//std::cout << bounds.at (i).x () << " " << bounds.at (i).y () << " / ";
		}
		
		//std::cout << std::endl;
	}
	
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
	painter->setBrush (Qt::red);
	painter->drawRect (rectangle);
	
	/*QPolygonF bounds = boundingPolygon ();
	painter->setBrush (Qt::green);
	
	for (qint32 i = 0; i < bounds.size (); i ++) {
		//qreal x = bounds.at (i).x (), y = bounds.at (i).y ();
		painter->drawEllipse (bounds.at (i), 2, 2);
	}*/
}

void Dron::slotMove () {
	//qreal dalpha = ::atan ((double) height / (double) width) / 2;
	qreal c  = ::cos (angle * Pi / 180), s  = ::sin (angle * Pi / 180);
	
	if (manual) {
		speed = 4;
		qreal x = point.x (), y = point.y ();
		
		if (GetAsyncKeyState ('W')) {
			x = point.x () + c * speed;
			y = point.y () + s * speed;
			
			if (!checkDrons (x, y)) { x -= c * speed; y -= s * speed;}
			if (!checkObstacles (x, y)) { x -= c * speed; y -= s * speed;}
		} else if (GetAsyncKeyState ('S')) {
			x = point.x () - c * speed;
			y = point.y () - s * speed;
			
			if (!checkDrons (x, y)) { x -= c * speed; y -= s * speed;}
			if (!checkObstacles (x, y)) { x -= c * speed; y -= s * speed;}
		}
		
		if (!checkBoundsX (x)) { x -= c * speed; }
		if (!checkBoundsY (y)) { y -= s * speed; }
		
		point.setX (x);
		point.setY (y);
		
		if (GetAsyncKeyState ('A') || GetAsyncKeyState ('D')) {
			qreal x = point.x ();
			qreal y = point.y ();
			qreal prev = angle;
			
			if (GetAsyncKeyState ('A')) { angle -= 2; }
			else                        { angle += 2; }
			if (angle > 180 || angle < -180) { normalizeAngle (); }
			
			if (!checkBoundsX (x)) { angle = prev; }
			if (!checkBoundsY (y)) { angle = prev; }
		}
	} else {
		qreal x = point.x () + c * speed;
		qreal y = point.y () + s * speed;
		
		if (!checkObstacles (x, y)) { x -= c * speed; y -= s * speed; angle += 67; }
		if (!checkBoundsX (x)) { x -= c * speed; angle += 67; }
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
}
