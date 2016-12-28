#include "gameform.h"
#include "ui_gameform.h"

GameForm::GameForm (Application* app) : QWidget (app), 
											ui (new Ui::gameform) {
	ui->setupUi(this);
	this->app = app;
	setDefault ();
	
	scene = new GameScene (this);
	scene->setSceneRect (-245, -245, 490, 490);
	
	ui->battlefield->setScene (scene);
	ui->battlefield->setRenderHint (QPainter::Antialiasing);
	ui->battlefield->setVerticalScrollBarPolicy   (Qt::ScrollBarAlwaysOff);
	ui->battlefield->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	ui->battlefield->setMouseTracking (true);
	
	connect (app, &Application::signalReceivedData,
			 this, &GameForm::slotReceivedData);
	
	qint32 w = scene->width ();
	qint32 h = scene->height ();
	
	scene->addLine (-w / 2, -h / 2, w / 2, -h / 2, QPen (Qt::black));
	scene->addLine (w / 2, -h / 2, w / 2, h / 2, QPen (Qt::black));
	scene->addLine (-w / 2, h / 2, w / 2, h / 2, QPen (Qt::black));
	scene->addLine (-w / 2, -h / 2, -w / 2, h / 2, QPen (Qt::black));
	
	QByteArray ask;
	QDataStream output (&ask, QIODevice::WriteOnly);
	QString command = "battle";    output << command;
	QString target  = "connected"; output << target;
	
	app->writeInSocket (ask);
	
	sendButtons = new QTimer (this);
	buttons.reset ();
	
	connect (sendButtons, &QTimer::timeout,
			 this, &GameForm::slotSendButtons);
	
	this->target = QPointF (0, 0);
	
	connect (scene, &GameScene::signalMove,
			 this, &GameForm::slotSetTarget);
	connect (scene, &GameScene::signalClick,
			 this, &GameForm::slotMakeShot);
	
	for (qint32 i = 0; i < 100; i ++) {
		BulletModel* bullet = new BulletModel (this, i);
		bullet->setPos (QPointF (-100000, -100000));
		scene->addItem (bullet);
		bullets.append (bullet);
	}
}

GameForm::~GameForm () {
	delete ui;
}

void GameForm::setDefault () {
	ui->timeGame->setText ("00:00");
	ui->pingServer->setText ("0 ms");
	
	ui->buttonLeave->setEnabled (false);
	ui->buttonPause->setEnabled (false);
	
	ui->player1HF->setText ("-");
	ui->player2HF->setText ("-");
	ui->player3HF->setText ("-");
	ui->player4HF->setText ("-");
	
	ui->player1HT->setText ("-");
	ui->player2HT->setText ("-");
	ui->player3HT->setText ("-");
	ui->player4HT->setText ("-");
	
	ui->player1PF->setText ("-");
	ui->player2PF->setText ("-");
	ui->player3PF->setText ("-");
	ui->player4PF->setText ("-");
	
	ui->player1PT->setText ("-");
	ui->player2PT->setText ("-");
	ui->player3PT->setText ("-");
	ui->player4PT->setText ("-");
	
	ui->turret->setStyleSheet    ("QLabel { background: none; }");
	ui->enclosure->setStyleSheet ("QLabel { background: none; }");
	ui->gun->setStyleSheet       ("QLabel { background: none; }");
}

void GameForm::updateData (QString target) {
	if (target == "time" || target == "all") {
		
	}
	
	if (target == "ping" || target == "all") {
		
	}
	
	if (target == "health" || target == "all") {
		if (heath.size () > 0) {
			QString h1 (QString ("%1").arg (heath.at (0).first));
			ui->player1HF->setText (h1);
			
			QString h2 (QString ("%1").arg (heath.at (0).second));
			ui->player1HT->setText (h2);
		}
		
		if (heath.size () > 1) {
			QString h1 (QString ("%1").arg (heath.at (1).first));
			ui->player2HF->setText (h1);
			
			QString h2 (QString ("%1").arg (heath.at (1).second));
			ui->player2HT->setText (h2);
		}
		
		if (heath.size () > 2) {
			QString h1 (QString ("%1").arg (heath.at (2).first));
			ui->player3HF->setText (h1);
			
			QString h2 (QString ("%1").arg (heath.at (2).second));
			ui->player3HT->setText (h2);
		}
		
		if (heath.size () > 3) {
			QString h1 (QString ("%1").arg (heath.at (3).first));
			ui->player4HF->setText (h1);
			
			QString h2 (QString ("%1").arg (heath.at (3).second));
			ui->player4HT->setText (h2);
		}
	}
	
	if (target == "points" || target == "all") {
		if (points.size () > 0) {
			QString h1 (QString ("%1").arg (points.at (0).first));
			ui->player1PF->setText (h1);
			
			QString h2 (QString ("%1").arg (points.at (0).second));
			ui->player1PT->setText (h2);
		}
		
		if (points.size () > 1) {
			QString h1 (QString ("%1").arg (points.at (1).first));
			ui->player2PF->setText (h1);
			
			QString h2 (QString ("%1").arg (points.at (1).second));
			ui->player2PT->setText (h2);
		}
	}
	
	if (target == "drons" || target == "all") {
		for (int i = 0; i < drons.size (); i ++) {
			DronModel* dron = drons.at (i);
			dron->setPos      (coords.at (i));
			dron->setRotation (angles.at (i));
		}
	}
}

qint32 GameForm::getAreaWidth () {
	return scene->width ();
}

qint32 GameForm::getAreaHeight () {
	return scene->height ();
}

bool GameForm::isActiveButton (int button) {
	return buttons.isAcive (button);
}

void GameForm::slotDisconnected () {
	emit signalSwitchForm (1);
}

void GameForm::slotButton (int button, int action) {
	buttons.setActive (button, action == 1);
}

void GameForm::slotReceivedData (QByteArray data) {
	QDataStream input (&data, QIODevice::ReadOnly);
	QString command; input >> command;
	
	if (command == "battle") {
		QString target; input >> target;
		
		if (target == "pre_countdown") {
			qint32 number; input >> number;
			angles.clear ();
			drons.clear  ();
			heath.clear  ();
			points.clear ();
			
			for (int i = 0; i < number; i ++) {
				qreal x, y;  input >> x >> y;
				coords.append (QPointF (x, y));
				
				qreal angle; input >> angle;
				angles.append (angle);
				turretAngles.append (0);
				
				DronModel* dron = new DronModel (this, i);
				scene->addItem (dron);
				drons.append (dron);
				
				qint32 maxHealth; input >> maxHealth;
				heath.append (std::make_pair (maxHealth, 
											  maxHealth));
			}
			
			qint32 vpoints; input >> vpoints;
			for (int i = 0; i < number; i ++) {
				points.append (std::make_pair (0, vpoints));
			}
			
			updateData ("all");
		} else if (target == "countdown") {
			qint32 time; input >> time;
			ui->timeGame->setText (QString (QString ("-00:%1%2")
												.arg (time > 9 ? "" : "0")
												.arg (time)));
		} else if (target == "game_start") {
			connect (scene, &GameScene::signalButton,
					 this, &GameForm::slotButton);
			
			sendButtons->start (1000 / 30);
			playing = true;
		} else if (target == "game_info") {
			qint32 minutes; input >> minutes;
			qint32 seconds; input >> seconds;
			ui->timeGame->setText (QString (QString ("%1:%2%3").arg(minutes)
															   .arg (seconds > 9 ? "" : "0")
															   .arg (seconds)));
			qint32 number; input >> number;
			
			for (int i = 0; i < number; i ++) {
				qreal x, y; input >> x >> y;
				coords.replace (i, QPointF (x, y));
				
				qreal angle; input >> angle;
				angles.replace (i, angle);
				
				qreal turretAngle; input >> turretAngle;
				turretAngles.replace (i, turretAngle);
				
				qint32 maxHealth;  input >> maxHealth;
				qint32 realHealth; input >> realHealth;
				heath.replace (i, std::make_pair (realHealth, 
												  maxHealth));
			}
			
			input >> number;
			
			for (qint32 i = 0; i < number; i ++) {
				qreal x, y; input >> x >> y;
				bullets.at (i)->setPos (QPointF (x, y));
				
				qreal angle; input >> angle;
				bullets.at (i)->setRotation (angle);
			}
			
			for (qint32 i = number; i < bullets.size (); i ++) {
				//bulletCoords.replace (i, QPointF (-100000, -100000));
				bullets.at (i)->setPos (QPointF (-100000, -100000));
				bullets.at (i)->setRotation (0);
			}
			
			updateData ("all");
		}
	}
}

void GameForm::slotSendButtons () {
	if (playing) {
		QByteArray ask;
		QDataStream output (&ask, QIODevice::WriteOnly);
		QString command = "battle";  output << command;
		QString target  = "buttons"; output << target;
									 //output << identf;
		output << buttons.isAcive (Qt::Key_W);
		output << buttons.isAcive (Qt::Key_A);
		output << buttons.isAcive (Qt::Key_D);
		
		output << (qreal) this->target.x ();
		output << (qreal) this->target.y ();
		
		app->writeInSocket (ask);
	}
}

void GameForm::slotSetTarget (QPointF target) {
	this->target = target;
}

void GameForm::slotMakeShot (int button) {
	if (button == Qt::LeftButton) {
		QByteArray ask;
		QDataStream output (&ask, QIODevice::WriteOnly);
		QString command = "battle"; output << command;
		QString target  = "shot";   output << target;
		
		output << (qreal) this->target.x ();
		output << (qreal) this->target.y ();
		
		app->writeInSocket (ask);
	}
}
