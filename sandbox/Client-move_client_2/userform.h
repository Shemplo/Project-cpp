#ifndef USERMENU_H
#define USERMENU_H

#include <Qtimer>
#include <QWidget>
#include <QDateTime>

class Application;

namespace Ui {
	class UserForm;
}

class UserForm : public QWidget {
		
	Q_OBJECT
		
	public:
		explicit UserForm (Application* app);
		~UserForm ();
		
		void setDefault ();
		void updateData (QString target);
	
	signals:
		void signalSwitchForm (int form);
		
	public slots:
		void slotConnected ();
		void slotDisconnected ();
		
	private slots:
		void slotButtonConnect ();
		void slotButtonExit    ();
		void slotButtonPrev    ();
		void slotButtonNext    ();
		void slotButtonBattle  ();
		
		void slotReconnect ();
		void slotReceivedData (QByteArray data);
		
		void slotPing ();
		
	private:
		Ui::UserForm* ui;
		Application* app;
		qint32 identif = 0;
		
		QTimer* reconnect;
		QTimer* ping;
		
		qint64 pingStarted;
		qint64 pingFinifhed;
		qint64 pingValue;
		bool tracingPing = false;
		
		struct Machine {
			qint32 id;
			QString name;
			
			qreal health, armor;
			qreal speed, rotate;
			qreal damage, duration;
			qreal bullet, capacity;
		};
		
		Machine machine;
		qint32 numberOfMachines = 0;
		qint32 numberOfMachine  = 0;
		
		qint32 credits    = 0;
		qint32 bonuses    = 0;
		qint32 experience = 0;
		
};

#endif // USERMENU_H
