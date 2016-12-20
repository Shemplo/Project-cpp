#ifndef DRAWCLIENT_H
#define DRAWCLIENT_H

#include <array>
#include <iostream>

#include <QTimer>
#include <QVector>
#include <QMainWindow>

#include <dron.h>
#include <gamescene.h>
#include <bullet.h>

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
		
		qint32 getAreaWidth  ();
		qint32 getAreaHeight ();
		
		bool isActiveButton (int button);
		void makeShot (Bullet* bullet);
		
	private:
		Ui::DrawClient*     ui;
		GameScene*          scene;
		QTimer*             timer;
		QVector <Dron *>    drons;
		QVector <QPolygonF> obstacles;
		
		struct Buttons {
			std::array <bool, 10> buffer;
			
			bool isAcive (int button) {
				if (button == Qt::Key_W) return buffer [0];
				if (button == Qt::Key_A) return buffer [1];
				if (button == Qt::Key_S) return buffer [2];
				if (button == Qt::Key_D) return buffer [3];
				
				return false;
			}
			
			void setActive (int button, bool state) {
				if (button == Qt::Key_W) buffer [0] = state;
				if (button == Qt::Key_A) buffer [1] = state;
				if (button == Qt::Key_S) buffer [2] = state;
				if (button == Qt::Key_D) buffer [3] = state;
			}
		};
		
		Buttons buttons;
		
	private slots:
		void slotButton (int button, int action);
		
};

#endif // DRAWCLIENT_H
