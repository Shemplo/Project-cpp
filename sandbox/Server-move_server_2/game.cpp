#include "game.h"
#include "client.h"
#include "machineinfo.h"

#define MInfo MachineInfo::Machine

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
	
	spawns.append (std::make_pair (-100, -100));
	spawns.append (std::make_pair (-100, 100));
	spawns.append (std::make_pair (100, -100));
	spawns.append (std::make_pair (100, 100));
}

bool Game::addPlayer (Client* client) {
	if (status == 0 && clients.size () < 4) {
		std::pair <qreal, 
				   qreal> pair = spawns.at (clients.size ());
		MInfo machine = client->getMachine ();
		
		Dron* dronInfo   = new Dron (); 
		dronInfo->x      = pair.first; 				 
		dronInfo->y      = pair.second;
		dronInfo->angle  = std::rand () % 30;
		dronInfo->health = machine.health;
		
		qint32 width  = machine.width;
		qint32 height = machine.height;
		dronInfo->width    = width;
		dronInfo->health   = height;
		dronInfo->diagonal = std::sqrt (width * width + height * height) / 2;
		
		dronInfo->speed = machine.speed;
		
		players.append   (dronInfo);
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
		output << clients.size ();
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			Dron* player = players.at (i);
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
	moveDrons ();
	
	QByteArray message;
	QDataStream output (&message, QIODevice::WriteOnly);
	output << QString ("battle");
	output << QString ("game_info");
	output << minutes << seconds;
	output << clients.size ();
	
	for (qint32 i = 0; i < clients.size (); i ++) {
		Dron* player = players.at (i);
		output << (qreal) player->x;
		output << (qreal) player->y;
		output << (qreal) player->angle;
		
		MInfo machine = clients.at (i)->getMachine ();
		output << (qint32) player->health;
		output << (qint32) machine.health;
	}
	
	for (qint32 i = 0; i < clients.size (); i ++) {
		clients.at (i)->writeInSocket (message);
	}
}

void Game::moveDrons () {
	for (qint32 i = 0; i < clients.size (); i ++) {
		qint32 width   = players.at (i)->width;
		qint32 height  = players.at (i)->health;
		qreal diagonal = players.at (i)->diagonal;
		qreal dalpha   = std::atan ((double) height / (double) width) / 2;
		
		qreal speed = players.at (i)->speed;
		qreal angle = players.at (i)->angle;
		qreal x = players.at (i)->x;
		qreal y = players.at (i)->y;
		
		qreal c = std::cos (angle * Pi / 180);
		qreal s = std::sin (angle * Pi / 180);
		
		bool f = players.at (i)->forward;
		if (f || true) {
			x += c * speed;
			y += s * speed;
		}
		
		if (!checkArenaBounds ("X", x, y, angle, diagonal, dalpha)) { x -= c * speed; }
		if (!checkArenaBounds ("Y", x, y, angle, diagonal, dalpha)) { y -= s * speed; }
		
		players.at (i)->x = x;
		players.at (i)->y = y;
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

qreal Game::rad (qreal angle) {
	return angle * Pi / 180;
}
