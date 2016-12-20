#include "drawclient.h"
#include "ui_drawclient.h"

DrawClient::DrawClient (QWidget* parent) : QMainWindow (parent),
											ui (new Ui::DrawClient) {
	ui->setupUi (this);
	this->resize (650, 550);
	this->setFixedSize (650, 550);
	
	scene = new GameScene ();
	timer = new QTimer ();
	
	ui->canvas->setMouseTracking (true);
	scene->setSceneRect (-300, -250, 600, 500);
	
	ui->canvas->setScene (scene);
	ui->canvas->setRenderHint (QPainter::Antialiasing);
	ui->canvas->setVerticalScrollBarPolicy   (Qt::ScrollBarAlwaysOff);
	ui->canvas->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	
	int w = scene->width ();
	int h = scene->height ();
	
	scene->addLine(-w / 2, 0, w / 2, 0, QPen (Qt::black));
	scene->addLine(0, -h / 2, 0, h / 2, QPen (Qt::black));
	
	scene->addLine(-w / 2, -h / 2, w / 2, -h / 2, QPen (Qt::black));
	scene->addLine(w / 2, -h / 2, w / 2, h / 2, QPen (Qt::black));
	scene->addLine(-w / 2, h / 2, w / 2, h / 2, QPen (Qt::black));
	scene->addLine(-w / 2, -h / 2, -w / 2, h / 2, QPen (Qt::black));
	
	QPolygon block;
	block << QPoint (110, 120) 
		  << QPoint (180, 90) 
		  << QPoint (120, 20)
		  << QPoint (50, 110);
	obstacles.append (block);
	scene->addPolygon (block);
	
	Dron* dron = new Dron ();
	connect (timer, &QTimer::timeout,
			 dron, &Dron::slotMove);
	
	dron->setManualControl (true);
	dron->setRectangleArea (w, h);
	dron->setDrawClient    (this);
	dron->id (0);
	
	drons.append (dron);
	scene->addItem (dron);
	
	Dron* bot  = new Dron ();
	connect (timer, &QTimer::timeout,
			 bot, &Dron::slotMove);
	
	bot->setManualControl (!true);
	bot->setRectangleArea (w, h);
	bot->setDrawClient    (this);
	bot->id (1);
	
	drons.append (bot);
	scene->addItem (bot);
	
	timer->setInterval (1000 / 60);
	timer->start ();
}

DrawClient::~DrawClient () {
	delete ui;
}

QVector <Dron *> DrawClient::getDrons () {
	return drons;
} 

QVector <QPolygonF> DrawClient::getObstacles () {
	return obstacles;
}
