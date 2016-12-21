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
	
	reconnect = new QTimer ();
	
	connect (reconnect, &QTimer::timeout,
			 this, &UserForm::slotReconnect);
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
	}
}

void UserForm::slotConnected () {
	ui->statusNow->setText ("online");
	ui->statusNow->setStyleSheet ("QLabel { color: green; }");
	
	connect (app, &Application::signalReceivedData,
			 this, &UserForm::slotReceivedData);
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "selected"; output << command;
	QString target  = "machine";  output << target;
	
	app->writeInSocket (ask);
}

void UserForm::slotDisconnected () {
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
	} else if (command == "selected") {
		QString target; input >> target;
		
		if (target == "machine") {
			//Ready to read information about selected machine;
			std::cout << "[SERVER] Machine data got!" 
					  << std::endl;
			
			input >> machine.name;
			input >> machine.health;
			input >> machine.armor;
			updateData ("machine");
		}
	}
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

UserForm::~UserForm() {
	delete ui;
}
