#include "game.h"
#include "client.h"
#include "machineinfo.h"

#define MInfo  MachineInfo::Machine
#define MpInfo MapInfo::Map

static const double Pi = 3.14159265358979323846264338327950288419717;

Game::Game (Server* server) : QObject (server) {
	std::cout << "[GAME] New game inited" 
			  << std::endl;
	
	this->server = server;
	
	stopwatch = new QTimer (this);
	connect (stopwatch, &QTimer::timeout,
			 this, &Game::slotSecondLeft);
	
	gameTimer = new QTimer (this);
	connect (gameTimer, &QTimer::timeout,
			 this, &Game::slotGameTick);
	
	MapInfo mapInfo;
	qint32 mapsNumber = mapInfo.getNumber ();
	selectedMap = std::rand () % mapsNumber;
	
	obstacles = mapInfo.getMap (selectedMap)
					   .obstacles;
	
	model  = new CollisionModel ();
	model1 = new CollisionModel ();
}

bool Game::addPlayer (Client* client) {
	if (status == 0 && clients.size () < 4) {
		QPointF pair = mapInfo.getMap(selectedMap)
							  .spawns
							  .at (clients.size ());
		MInfo machine = client->getMachine ();
		
		Dron* dronInfo   = new Dron  (); 
		dronInfo->x      = pair.x    (); 				 
		dronInfo->y      = pair.y    ();
		dronInfo->angle  = 0;
		dronInfo->health = machine.health;
		
		qint32 width  = machine.width;
		qint32 height = machine.height;
		dronInfo->width    = width;
		dronInfo->health   = height;
		dronInfo->diagonal = std::sqrt (width * width + height * height) / 2;
		
		dronInfo->speed    = machine.speed;
		dronInfo->duration = machine.duration;
		dronInfo->bullet   = machine.bullet;
		dronInfo->capacity = machine.capacity;
		dronInfo->penetration = machine.penetration;
		
		drons.append   (dronInfo);
		clients.append   (client);
		readyMask.append (false);
		
		connect (client, &Client::signalGameReady,
				 this, &Game::slotClientReady);
		
		return true;
	}
	
	return false;
}

void Game::closeAdding () {
	if (clients.size () > 1) {
		std::cout << "[GAME] Adding closed... wait for countdown" 
				  << std::endl;
		
		QByteArray message;
		QDataStream output (&message, QIODevice::WriteOnly);
		output << QString ("battle");
		output << QString ("joined_game");
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			clients.at (i)->writeInSocket (message);
		}
		
		status = 1;
		if (!stopwatch->isActive ()) {
			std::cout << "stopwatch" << std::endl;
			stopwatch->start (1000);
		}
	} else if (!isSingle) {
		//To few players in room
		std::cout << "[GAME] Game closed due to few" 
				  << " players after closing adding" 
				  << std::endl;
		
		QByteArray message;
		QDataStream output (&message, QIODevice::WriteOnly);
		output << QString ("battle");
		output << QString ("close_game");
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			clients.at (i)->writeInSocket (message);
		}
		
		server->closeGame (this);
	}
}

void Game::startGame () {
	QByteArray message;
	QDataStream output (&message, QIODevice::WriteOnly);
	output << QString ("battle");
	output << QString ("game_start");
	
	for (qint32 i = 0; i < clients.size (); i ++) {
		clients.at (i)->writeInSocket (message);
	}
	
	gameTimer->start (1000 / 120);
	playing = true;
}

void Game::slotClientReady (qint32 id) {
	qint32 index = -1;
	for (qint32 i = 0; i < clients.size (); i ++) {
		if (clients.at (i)->getId () == id) {
			index = i;
			break;
		}
	}
	
	if (index != -1) {
		readyMask.replace (index, true);
	}
	
	bool flag = true; int a = 0;
	for (qint32 i = 0; i < readyMask.size (); i ++) {
		if (!readyMask.at (i)) { 
			flag = false;
			a ++;
			//break;
		}
	}
	
	if (flag) { status = 2; }
}

void Game::slotSecondLeft () {
	if (status == 2 && countdown < 0) { 
		std::cout << "[GAME] Countdown ..." 
				  << std::endl;
		
		QByteArray message;
		QDataStream output (&message, QIODevice::WriteOnly);
		output << QString ("battle");
		output << QString ("pre_countdown");
		
		output << obstacles.size ();
		
		for (qint32 i = 0; i < obstacles.size (); i ++) {
			output << (QPolygonF) obstacles.at (i);
		}
		
		output << clients.size ();
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			Dron* player = drons.at (i);
			output << (qreal) player->x;
			output << (qreal) player->y;
			output << (qreal) player->angle;
			
			MInfo machine = clients.at (i)->getMachine ();
			output << (qint32) machine.health;
		}
		
		output << 200;
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			clients.at (i)->writeInSocket (message);
		}
		
		countdown = 10;
	}
	
	if (status == 2 && countdown > 0) {
		QByteArray message;
		QDataStream output (&message, QIODevice::WriteOnly);
		output << QString ("battle");
		output << QString ("countdown");
		output << countdown;
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			clients.at (i)->writeInSocket (message);
			//TODO: check for disconnected
		}
		
		countdown --;
	}
	
	if (status == 2 && countdown == 0) {
		status = 3; countdown = -1;
		startGame ();
	}
	
	if (status == 3) {
		minutes += (seconds + 1) / 60;
		seconds = (seconds + 1) % 60;
	}
}

void Game::slotGameTick () {
	moveDrons   ();
	moveBullets ();
	
	QByteArray message;
	QDataStream output (&message, QIODevice::WriteOnly);
	output << QString ("battle");
	output << QString ("game_info");
	output << minutes << seconds;
	output << clients.size ();
	
	for (qint32 i = 0; i < clients.size (); i ++) {
		Dron* player = drons.at (i);
		output << (qreal) player->x;
		output << (qreal) player->y;
		output << (qreal) player->angle;
		output << (qreal) player->turretAngle;
		
		MInfo machine = clients.at (i)->getMachine ();
		output << (qint32) player->health;
		output << (qint32) machine.health;
	}
	
	output << bullets.size ();
	
	for (qint32 i = 0; i < bullets.size (); i ++) {
		Bullet* bullet = bullets.at (i);
		output << (qreal) bullet->x;
		output << (qreal) bullet->y;
		output << (qreal) bullet->angle;
	}
	
	for (qint32 i = 0; i < clients.size (); i ++) {
		clients.at (i)->writeInSocket (message);
	}
}

void Game::moveDrons () {
	//Move drons
	for (qint32 i = 0; i < clients.size (); i ++) {
		qint32 width   = drons.at (i)->width;
		qint32 height  = drons.at (i)->health;
		qreal diagonal = drons.at (i)->diagonal;
		qreal dalpha   = std::atan ((double) height / (double) width) / 2;
		
		qreal speed = drons.at (i)->speed;
		qreal angle = drons.at (i)->angle;
		qreal x = drons.at (i)->x;
		qreal y = drons.at (i)->y;
		
		qreal c = std::cos (rad (angle));
		qreal s = std::sin (rad (angle));
		
		if (clients.at (i)->forward) {
			x += c * speed;
			y += s * speed;
			
			if (!checkObstacles ("", x, y, 
								 angle, 
								 width, height, 
								 dalpha)) { 
				x -= c * speed; y -= s * speed; 
			}
		}
		
		if (!checkArenaBounds ("X", x, y, angle, diagonal, dalpha)) { x -= c * speed; }
		if (!checkArenaBounds ("Y", x, y, angle, diagonal, dalpha)) { y -= s * speed; }
		
		drons.at (i)->x = x;
		drons.at (i)->y = y;
		
		if (clients.at (i)->left || clients.at (i)->right) {
			//qreal x = drons.at (i)->x;
			//qreal y = drons.at (i)->y;
			//qreal prev = angle;
			
			if (clients.at (i)->left) { angle -= 2; }
			else                      { angle += 2; }
			if (angle > 180 || angle < -180) { angle = normalizeAngle (angle); }
			
			/*if (!checkArenaBounds ("X", x, y, angle, diagonal, dalpha)) {
				angle = prev + (angle - prev) / 2;
			}
			
			if (!checkArenaBounds ("Y", x, y, angle, diagonal, dalpha)) {
				angle = prev + (angle - prev) / 2;
			}*/
		}
		
		drons.at (i)->angle = angle;
		
		QLineF toTarget (QPointF (x, y), clients.at (i)->target);
		qreal angleToTarget = -toTarget.angle ();
		
		//std::cout << x << " " << y << " " << clients.at (i)->target.x () << " " << angleToTarget << std::endl;
		if (std::abs (angleToTarget - angle) >= 0) { drons.at (i)->turretAngle = angleToTarget - angle; } 
		else                                       { drons.at (i)->turretAngle = angle - angleToTarget; }
		
		qreal timeShot = drons.at (i)->duration;
		if (drons.at (i)->timeout >= gameTimer->interval () * timeShot) {
			x = drons.at (i)->x;
			y = drons.at (i)->y;
						
			for (qint32 j = 0; j < clients.at (i)->shots.size (); j ++) {
				if (drons.at (i)->capacity > 0) {
					QPointF targetCoords  = clients.at (i)->shots.front ();
					QLineF  toTargetLine  (QPointF (x, y), targetCoords);
					qreal   angleToTarget = -toTargetLine.angle ();
					
					Bullet* bullet = new Bullet ();
					bullet->x = x; bullet->y = y;
					bullet->angle  = angleToTarget;
					bullet->speed  = drons.at (i)->bullet;
					bullet->damage = drons.at (i)->damage;
					bullet->penetration = drons.at (i)->penetration;
					
					bullet->fromId = i;
					bullets.append (bullet);
					
					drons.at (i)->capacity --;
					clients.at (i)->shots.pop_front ();
					
					drons.at (i)->timeout = -1;
				}
			}
		} else {
			clients.at (i)->shots.clear ();
		}
		
		qint32 t = drons.at (i)->timeout;
		drons.at (i)->timeout = t < 1024 ? t + 1 : t;
	}
}

void Game::moveBullets () {
	//Move bullets
	for (qint32 i = 0; i < bullets.size (); i ++) {
		qint32 width   = bullets.at (i)->width;
		qint32 height  = bullets.at (i)->height;
		qreal diagonal = std::sqrt (width * width + height * height) / 2;
		qreal dalpha   = std::atan ((double) height / (double) width) / 2;
		
		qreal speed = bullets.at (i)->speed;
		qreal angle = bullets.at (i)->angle;
		qreal x = bullets.at (i)->x;
		qreal y = bullets.at (i)->y;
		
		angle = normalizeAngle (angle);
		bullets.at (i)->angle = angle;
		
		x += std::cos (rad (angle)) * speed;
		y += std::sin (rad (angle)) * speed;
		
		if (!checkObstacles ("", x, y, 
							 angle, 
							 width, height, 
							 dalpha)) { 
			bullets.remove (i); i --;
			continue;
		}
		
		qint32 shotedDron = checkDrons ("", x, y, angle,
										width, height,
										dalpha);
		if (shotedDron != -1) {
			if (bullets.at (i)->fromId != shotedDron) {
				bullets.remove (i); i --;
				continue;
			}
		}
		
		if (!checkArenaBounds ("X", x, y, angle, diagonal, dalpha)) {
			bullets.remove (i); i --;
			continue;
		}
		
		if (!checkArenaBounds ("Y", x, y, angle, diagonal, dalpha)) {
			bullets.remove (i); i --;
			continue;
		}
		
		bullets.at (i)->x = x;
		bullets.at (i)->y = y;
	}
}

bool Game::checkArenaBounds (QString asix, 
							 qreal x, 
							 qreal y, 
							 qreal angle, 
							 qreal diagonal,
							 qreal dalpha) {
	if (asix == "X") {
		if (angle >= -180 && angle < -90) {
			if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) - dalpha)) > awidth / 2) { return false; }
		} else if (angle >= -90 && angle < 0) {
			if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) + dalpha)) > awidth / 2) { return false; }
		} else if (angle >= 0 && angle < 90) {
			if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) - dalpha)) > awidth / 2) { return false; }
		} else if (angle >= 90 && angle <= 180) {
			if (std::abs (x + (diagonal + 1) * std::cos (rad (angle) + dalpha)) > awidth / 2) { return false; }
		}
	} else if (asix == "Y") {
		if (angle >= -180 && angle < -90) {
			if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) + dalpha)) > aheight / 2) { return false; }
		} else if (angle >= -90 && angle < 0) {
			if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) - dalpha)) > aheight / 2) { return false; }
		} else if (angle >= 0 && angle < 90) {
			if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) + dalpha)) > aheight / 2) { return false; }
		} else if (angle >= 90 && angle <= 180) {
			if (std::abs (y + (diagonal + 1) * std::sin (rad (angle) - dalpha)) > aheight / 2) { return false; }
		}
	}
	
	return true;
}

bool Game::checkObstacles (QString asix, 
						   qreal x, qreal y,
						   qreal angle,
						   qreal width, qreal height,
						   qreal dalpha) {
	Q_UNUSED (asix);
	
	model->setBounds (QRectF (x, y, width, height));
	QPolygonF bounds = model->boundingPolygon (x, y, 
											   angle, 
											   dalpha);
	
	for (qint32 i = 0; i < obstacles.size (); i ++) {
		if (!obstacles.at (i)
					  .intersected (bounds)
					  .isEmpty ()) {
			return false;
		}
	}
	
	return true;
}

qint32 Game::checkDrons (QString asix, 
					    qreal x, qreal y,
					    qreal angle,
					    qreal width, qreal height,
					    qreal dalpha) {
	Q_UNUSED (asix);
	
	model->setBounds (QRectF (x, y, width, height));
	QPolygonF bounds = model->boundingPolygon (x, y, 
											   angle, 
											   dalpha);
	
	for (qint32 i = 0; i < drons.size (); i ++) {
		Dron* dron = drons.at (i);
		model1->setBounds (QRectF (dron->x, dron->y, 
								  dron->width, dron->health));\
		qreal daplha1 = std::atan ((double) dron->health 
								   / (double) dron->width) / 2;
		QPolygonF bounds1 = model1->boundingPolygon (dron->x, dron->y,
													dron->angle, daplha1);
		
		if (!bounds1.intersected (bounds).isEmpty ()) { return i; }
	}
	
	return -1;
}

qreal Game::normalizeAngle (qreal angle) {
	while (angle > 180) { angle -= 360; }
	while (angle < -180) { angle += 360; }
	
	return angle;
}

qreal Game::rad (qreal angle) {
	return angle * Pi / 180;
}
