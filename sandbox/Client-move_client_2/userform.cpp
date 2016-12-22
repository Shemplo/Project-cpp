#include "userform.h"
#include "ui_userform.h"

#include <application.h>

UserForm::UserForm (Application* app) : QWidget (app), 
										ui (new Ui::UserForm) {
	ui->setupUi (this);
	this->app = app;
	setDefault ();
	
	connect (ui->buttonConnect, &QPushButton::clicked,
			 this, &UserForm::slotButtonConnect);
	connect (ui->buttonExit, &QPushButton::clicked,
			 this, &UserForm::slotButtonExit);
	connect (ui->buttonPrev, &QPushButton::clicked,
			this, &UserForm::slotButtonPrev);
	connect (ui->buttonNext, &QPushButton::clicked,
			 this, &UserForm::slotButtonNext);
	connect (ui->buttonBattle, &QPushButton::clicked,
			 this, &UserForm::slotButtonBattle);
	
	reconnect = new QTimer ();
	ping      = new QTimer ();
	
	connect (reconnect, &QTimer::timeout,
			 this, &UserForm::slotReconnect);
	
	if (app->isConnected ()) {
		slotConnected ();
	}
}

void UserForm::setDefault () {
	ui->statusNow->setText ("offline");
	ui->statusNow->setStyleSheet ("QLabel { color: red; }");
	
	ui->buttonConnect->setEnabled (true);
	ui->buttonBattle->setEnabled (false);
	ui->buttonResearch->setEnabled (false);
	ui->buttonShop->setEnabled (false);
	
	ui->creditsAmong->setText ("-");
	ui->bonucesAmong->setText ("-");
	ui->experienceAmong->setText ("-");
	ui->timeUser->setText ("00:00");
	
	ui->machineName->setText ("-");
	ui->healthAmong->setText ("-");
	ui->armorAmong->setText ("-");
	ui->speedAmong->setText ("-");
	ui->rotateSpeedAmong->setText ("-");
	ui->damageAmong->setText ("-");
	ui->attackSpeedAmong->setText ("-");
	ui->bulletSpeedAmong->setText ("-");
	ui->ammoCapacityAmong->setText ("-");
	
	ui->buttonPrev->setEnabled (false);
	ui->buttonNext->setEnabled (false);
	
	ui->machinePreview->setEnabled (false);
}

void UserForm::updateData (QString target) {
	if (!app->isConnected ()) { return; }
	
	if (target == "machine" || target == "all") {
		ui->machineName->setText (machine.name);
		ui->healthAmong->setText (QString (QString ("%1").arg (machine.health)));
		ui->armorAmong->setText (QString (QString ("%1").arg (machine.armor)));
		ui->speedAmong->setText (QString (QString ("%1").arg (machine.speed)));
		ui->rotateSpeedAmong->setText (QString (QString ("%1").arg (machine.rotate)));
		ui->damageAmong->setText (QString (QString ("%1").arg (machine.damage)));
		ui->attackSpeedAmong->setText (QString (QString ("%1").arg (machine.duration)));
		ui->bulletSpeedAmong->setText (QString (QString ("%1").arg (machine.bullet)));
		ui->ammoCapacityAmong->setText (QString (QString ("%1").arg (machine.capacity)));
		
		if (numberOfMachines > 1) {
			if (numberOfMachine == numberOfMachines - 1) {
				ui->buttonPrev->setEnabled (true);
				ui->buttonNext->setEnabled (false);
			} else if (numberOfMachine == 0) {
				ui->buttonPrev->setEnabled (false);
				ui->buttonNext->setEnabled (true);
			} else {
				ui->buttonPrev->setEnabled (true);
				ui->buttonNext->setEnabled (true);
			}
		} else {
			ui->buttonPrev->setEnabled (false);
			ui->buttonNext->setEnabled (false);
		}
		
		ui->buttonBattle->setEnabled (true);
	}
	
	if (target == "balance" || target == "all") {
		ui->creditsAmong->setText (QString (QString ("%1").arg (credits)));
		ui->bonucesAmong->setText (QString (QString ("%1").arg (bonuses)));
		ui->experienceAmong->setText (QString (QString ("%1").arg (experience)));
		
		ui->buttonResearch->setEnabled (true);
		ui->buttonShop->setEnabled (true);
	}
	
	if (target == "ping" || target == "all") {
		ui->pingMs->setText (QString (QString ("%1 ms").arg (pingValue)));
	}
	
	if (target == "time" || target == "all") {
		QString time (QDateTime::currentDateTime ().
						toString ("hh:mm"));
		ui->timeUser->setText (time);
	}
}

void UserForm::slotConnected () {
	ui->buttonConnect->setEnabled (false);
	
	ui->statusNow->setText ("online");
	ui->statusNow->setStyleSheet ("QLabel { color: green; }");
	
	connect (app, &Application::signalReceivedData,
			 this, &UserForm::slotReceivedData);
	
	QByteArray askB;
	QDataStream outputB (&askB, QIODevice::WriteOnly);
	QString commandB = "selected"; outputB << commandB;
	QString targetB  = "balance";  outputB << targetB;
	
	app->writeInSocket (askB);
	
	QByteArray askM;
	QDataStream outputM (&askM, QIODevice::WriteOnly);
	QString commandM = "selected"; outputM << commandM;
	QString targetM  = "machine";  outputM << targetM;
	
	app->writeInSocket (askM);
	
	ping->start (1000);
	connect (ping, &QTimer::timeout,
			 this, &UserForm::slotPing);
}

void UserForm::slotDisconnected () {
	ui->buttonConnect->setEnabled (true);
	
	ping->stop ();
	pingValue = 0;
	
	setDefault ();
}

void UserForm::slotReconnect () {
	if (!app->isConnected ()) { setDefault (); }
	reconnect->stop ();
}

void UserForm::slotReceivedData (QByteArray data) {
	QDataStream input (&data, QIODevice::ReadOnly);
	QString command; input >> command;
	
	if (command == "pong") {
		std::cout << "[SERVER] We got pong!" 
				  << std::endl;
	} else if (command == "client_pong") {
		qint32 clientId; input >> clientId;
		
		if (tracingPing && clientId == identif) {
			tracingPing = false;
			
			pingFinifhed = QDateTime::currentDateTime ().toMSecsSinceEpoch ();
			pingValue = pingFinifhed - pingStarted;
			
			updateData ("ping");
		}
	} else if (command == "selected") {
		QString target; input >> target;
		
		if (target == "machine") {
			//Ready to read information about selected machine
			std::cout << "[CLIENT] Machine data received" 
					  << std::endl;
			
			input >> machine.name;
			input >> machine.id;
			
			if (machine.id != 0) {
				input >> machine.health;
				input >> machine.armor;
				input >> machine.speed;
				input >> machine.rotate;
				input >> machine.damage;
				input >> machine.duration;
				input >> machine.bullet;
				input >> machine.capacity;
				input >> numberOfMachines;
				input >> numberOfMachine;
				
				updateData ("machine");
			}
		} else if (target == "balance") {
			//Ready to read information about user balance
			std::cout << "[CLIENT] User balance data received" 
					  << std::endl;
			
			input >> credits;
			input >> bonuses;
			input >> experience;
			input >> identif;
			
			updateData ("balance");
		}
	} else if (command == "battle") {
		QString target; input >> target;
		
		if (target == "joined_queue") {
			emit signalSwitchForm (2);
		}
	}
}

void UserForm::slotPing () {
	pingStarted = QDateTime::currentDateTime ().
					toMSecsSinceEpoch ();
	tracingPing = true;
	updateData ("time");
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	
	//Asking server for a ping test
	output << QString ("client_ping");
	output << identif;
	
	app->writeInSocket (ask);
}

void UserForm::slotButtonConnect () {
	std::cout << "[CLIENT] Connection button" 
			  << std::endl;
	
	ui->buttonConnect->setEnabled (false);
	app->connectHost ("127.0.0.1", 16310);
	
	reconnect->start (3000);
}

void UserForm::slotButtonExit () {
	app->closeApplication ();
}

void UserForm::slotButtonPrev () {
	std::cout << "[CLIENT] Switch to previous machine" 
			  << std::endl;
	ui->buttonPrev->setEnabled (false);
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "prev";    output << command;
	QString target  = "machine"; output << target;
	
	app->writeInSocket (ask);
}

void UserForm::slotButtonNext () {
	std::cout << "[CLIENT] Switch to next machine" 
			  << std::endl;
	ui->buttonNext->setEnabled (false);
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "next";    output << command;
	QString target  = "machine"; output << target;
	
	app->writeInSocket (ask);
}

void UserForm::slotButtonBattle () {
	std::cout << "[CLIENT] Join to queue to battle" 
			  << std::endl;
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "battle";     output << command;
	QString target  = "join_queue"; output << target;
	
	app->writeInSocket (ask);
}

UserForm::~UserForm() {
	delete ui;
}
