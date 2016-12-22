#include "queueform.h"
#include "ui_queueform.h"

QueueForm::QueueForm (Application* app) : QWidget (app),
											ui (new Ui::QueueForm) {
	ui->setupUi (this);
	this->app = app;
	setDefault ();
	
	connect (app, &Application::signalReceivedData,
			 this, &QueueForm::slotReceivedData);
	
	time = new QTimer (this);
	
	connect (time, &QTimer::timeout,
			 this, &QueueForm::slotPlusSecond);
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "queue"; output << command;
	QString target  = "info";  output << target;
	
	app->writeInSocket (ask);
	
	connect (ui->buttonLeave, &QPushButton::clicked,
			 this, &QueueForm::slotButtonLeave);
}

void QueueForm::setDefault () {
	ui->queueTime->setText ("00:00");
	ui->playersNumber->setText ("0");
	ui->machineName->setText ("-");
	ui->machineLevel->setText ("- level");
	ui->buttonTryAlone->setEnabled (false);
}

void QueueForm::updateData (QString target) {
	if (!app->isConnected ()) {
		emit signalSwitchForm (1);
	}
	
	if (target == "time" || target == "all") {
		qint32 seconds = timeIn % 60;
		qint64 minutes = timeIn / 60;
		
		ui->queueTime->setText (QString (QString ("%1:%2").
											arg (minutes).
											arg (seconds)));
	}
	
	if (target == "queue" || target == "all") {
		ui->playersNumber->setText (QString (QString ("%1").arg (users)));
		ui->machineLevel->setText (QString (QString ("%1 level").arg (level)));
		ui->machineName->setText (machineName);
	}
}

void QueueForm::slotDisconnected () {
	emit signalSwitchForm (1);
}

void QueueForm::slotReceivedData (QByteArray data) {
	QDataStream input (&data, QIODevice::ReadOnly);
	QString command; input >> command;
	
	if (command == "queue") {
		QString target; input >> target;
		
		if (target == "info") {
			if (!time->isActive ()) { time->start (1000); }
			
			input >> users;
			input >> id >> level;
			input >> machineName;
			
			updateData ("all");
		} else if (target == "users") {
			if (!time->isActive ()) { time->start (1000); }
			
			input >> users;
			
			updateData ("all");
		} else if (target == "leaved") {
			time->stop ();
			emit signalSwitchForm (1);
		}
	}
}

void QueueForm::slotPlusSecond () {
	timeIn = timeIn + 1 > 0 
				? timeIn + 1 
				: 0;
	updateData ("time");
}

void QueueForm::slotButtonLeave () {
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "queue"; output << command;
	QString target  = "leave";  output << target;
	
	app->writeInSocket (ask);
}

QueueForm::~QueueForm () {
	delete ui;
}
