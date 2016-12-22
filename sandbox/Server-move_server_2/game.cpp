#include "game.h"
#include "client.h"
#include "machineinfo.h"

#define MInfo MachineInfo::Machine

Game::Game (Server* server) : QObject (server) {
	std::cout << "[GAME] New game inited" 
			  << std::endl;
	
	this->server = server;
	
	stopwatch = new QTimer (this);
}

bool Game::addPlayer (Client* client) {
	if (status == 0 && clients.size () < 4) {
		clients.append   (client);
		readyMask.append (false);
		
		connect (client, &Client::signalGameReady,
				 this, &Game::slotClientReady);
		
		//QByteArray message;
		//QDataStream output (&message, QIODevice::WriteOnly);
		//output << QString ("battle");
		//output << QString ("joined_game");
		
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
	
	bool flag = true;
	for (qint32 i = 0; i < readyMask.size (); i ++) {
		if (!readyMask.at (i)) { 
			flag = false;
			break;
		}
	}
	
	if (flag) { status = 1; }
}

void Game::slotSecondLeft () {
	if (status == 1 && countdoun < 0) { countdoun = 30; }
	
	if (status == 1 && countdoun > 0) {
		QByteArray message;
		QDataStream output (&message, QIODevice::WriteOnly);
		output << QString ("battle");
		output << QString ("countdown");
		output << countdoun;
		
		for (qint32 i = 0; i < clients.size (); i ++) {
			clients.at (i)->writeInSocket (message);
		}
	}
	
	if (status == 1 && countdoun == 0) {
		status = 2; countdoun = -1;
		startGame ();
	}
}
