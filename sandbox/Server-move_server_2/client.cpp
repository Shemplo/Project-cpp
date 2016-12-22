#include "client.h"
#include "server.h"

#define MInfo MachineInfo::Machine

Client::Client (QTcpSocket* socket, qint32 id, Server* server) : QObject (server) {
	this->status = 0;      //Just connected to server
	this->identf = id;
	this->server = server;
	this->socket = socket;
	
	connect (this->socket, &QTcpSocket::readyRead,
			 this, &Client::slotReadStream);
	connect (this->socket, &QTcpSocket::disconnected,
			 this, &Client::slotDisconnect);
	
	machines.append (1);
	machines.append (2);
}

void Client::writeInSocket (QByteArray data) {
	QByteArray length;
	QDataStream lenout (&length, QIODevice::WriteOnly);
	lenout << data.size ();
	
	socket->write (length);
	socket->write (data);
}

qint32 Client::getId () {
	return this->identf;
}

void Client::slotReadStream () {
	while (socket->bytesAvailable () > 0) {
		bufferArray.append (socket->readAll ());
		
		while (bufferArray.size () >= sizeof (qint32) && need == 0) {
			QDataStream input (&bufferArray, QIODevice::ReadOnly);
			/*Reading size of input data line*/ input >> need;
			bufferArray.remove (0, sizeof (qint32));
			
			if (bufferArray.size () >= need && need != 0) {
				parseInputStream (bufferArray.left (need));
				bufferArray.remove (0, need);
				need = 0;
			}
		}
		
		//if (bufferArray.size () >= sizeof (qint32) && need == 0) {
		//	QDataStream input (&bufferArray, QIODevice::ReadOnly);
		//	/*Reading size of input data line*/ input >> need;
		//	bufferArray.remove (0, sizeof (qint32));
		//}
		
		//if (bufferArray.size () >= need && need != 0) {
		//	std::cout << bufferArray.left (need).toStdString () 
		//			  << std::endl;
		//	parseInputStream (bufferArray.left (need));
		//	bufferArray.remove (0, need);
		//	need = 0;
		//}
	}
}

void Client::parseInputStream (QByteArray data) {
	QDataStream input (&data, QIODevice::ReadOnly);
	QString command; input >> command;
	
	if (command != "client_ping") {
		std::cout << "[SOCKET] Received command: " 
				  << command.toStdString () 
				  << std::endl;
	}
	
	if (command == "ping" || command == "client_ping") {
		QByteArray pong, length;
		QDataStream output (&pong, QIODevice::WriteOnly);
		
		if (command == "client_ping") {
			qint32 clientId; input >> clientId;
			output << QString ("client_pong");
			output << clientId;
		} else {
			output << QString ("pong");
		}
		
		QDataStream lenout (&length, QIODevice::WriteOnly);
		lenout << pong.size ();
		
		socket->write (length);
		socket->write (pong);
	} else if (command == "selected") {
		QString target; input >> target;
		
		if (target == "machine") {
			std::cout << "[CLIENT] Sending data of selected machine number " 
					  << this->selectedMachine << std::endl;
			
			sendMachineInfo (selectedMachine);
		} else if (target == "balance") {
			std::cout << "[CLIENT] Sending data of user balance" 
					  << std::endl;
			
			sendBalanceInfo ();
		}
	} else if (command == "prev") {
		QString target; input >> target;
		
		if (target == "machine") {
			if (selectedMachine - 1 >= 0) {
				selectedMachine --;
			}
			
			std::cout << "[CLIENT] Sending data of selected machine number " 
					  << this->selectedMachine << std::endl;
			
			sendMachineInfo (selectedMachine);
		}
	} else if (command == "next") {
		QString target; input >> target;
		
		if (target == "machine") {
			if (selectedMachine + 1 < machines.size ()) {
				selectedMachine ++;
			}
			
			std::cout << "[CLIENT] Sending data of selected machine number " 
					  << this->selectedMachine << std::endl;
			
			sendMachineInfo (selectedMachine);
		}
	} else if (command == "battle") {
		QString target; input >> target;
		
		if (target == "join_queue") {
			std::cout << "[SERVER] Client #" << identf 
					  << " joined queue" << std::endl;
			QByteArray pong, length;
			QDataStream output (&pong, QIODevice::WriteOnly);
			output << QString ("battle");
			output << QString ("joined_queue");
			
			QDataStream lenout (&length, QIODevice::WriteOnly);
			lenout << pong.size ();
			
			socket->write (length);
			socket->write (pong);
			
			server->joinToBattle (this);
			status = 1;
		}
	} else if (command == "queue") {
		QString target; input >> target;
		
		if (target == "info") {
			QByteArray pong, length;
			QDataStream output (&pong, QIODevice::WriteOnly);
			output << QString ("queue");
			output << QString ("info");
			
			MInfo machine = machineInfo.getInfo (machines.at (selectedMachine));
			
			output << server->queueSize ();
			output << machine.id;
			output << machine.level;
			output << machine.name;
			
			QDataStream lenout (&length, QIODevice::WriteOnly);
			lenout << pong.size ();
			
			socket->write (length);
			socket->write (pong);
		} else if (target == "leave") {
			std::cout << "[SERVER] Client #" << identf 
					  << " leaved queue" << std::endl;
			server->leaveQueue (this);
			
			QByteArray pong, length;
			QDataStream output (&pong, QIODevice::WriteOnly);
			output << QString ("queue");
			output << QString ("leaved");
			
			QDataStream lenout (&length, QIODevice::WriteOnly);
			lenout << pong.size ();
			
			socket->write (length);
			socket->write (pong);
		}
	}
}

void Client::sendMachineInfo (qint32 id) {
	qint32 machineId = 0;
	MInfo  machine   = machineInfo.getInfo (-1);
	
	if (id >= 0 && id < machines.size ()) {
		machineId = machines.at (id);
		machine   = machineInfo.getInfo (machineId);
	}
	
	QByteArray pong, length;
	QDataStream output (&pong, QIODevice::WriteOnly);
	output << QString ("selected");
	output << QString ("machine");
	
	//Writing data about machine
	output << QString (machine.name);
	output << (qint32) machine.id;
	output << (qreal)  machine.health;
	output << (qreal)  machine.armor;
	output << (qreal)  machine.speed;
	output << (qreal)  machine.rotate;
	output << (qreal)  machine.damage;
	output << (qreal)  machine.duration;
	output << (qreal)  machine.bullet;
	output << (qreal)  machine.capacity;
	
	//Writing data about number 
	//of user machines
	output << machines.size ();
	output << selectedMachine;
	
	QDataStream lenout (&length, QIODevice::WriteOnly);
	lenout << pong.size ();
	
	socket->write (length);
	socket->write (pong);
}

void Client::sendBalanceInfo () {
	QByteArray pong, length;
	QDataStream output (&pong, QIODevice::WriteOnly);
	output << QString ("selected");
	output << QString ("balance");
	
	//Writing data about user balance
	output << credits;
	output << bonuses;
	output << experience;
	output << identf;
	
	QDataStream lenout (&length, QIODevice::WriteOnly);
	lenout << pong.size ();
	
	socket->write (length);
	socket->write (pong);
}

void Client::slotDisconnect () {
	server->removeClient (this);
	socket->close ();
	delete this;
}
