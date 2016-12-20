#ifndef DRAWCLIENT_H
#define DRAWCLIENT_H

#include <iostream>

#include <QTimer>
#include <QVector>
#include <QMainWindow>

#include <dron.h>
#include <gamescene.h>

namespace Ui {
	class DrawClient;
}

class DrawClient : public QMainWindow {
		
	Q_OBJECT
		
	public:
		explicit DrawClient (QWidget *parent = 0);
		~DrawClient ();
		
		QVector <Dron *>    getDrons ();
		QVector <QPolygonF> getObstacles ();
		
	private:
		Ui::DrawClient*     ui;
		GameScene*          scene;
		QTimer*             timer;
		QVector <Dron *>    drons;
		QVector <QPolygonF> obstacles;
		
};

#endif // DRAWCLIENT_H
