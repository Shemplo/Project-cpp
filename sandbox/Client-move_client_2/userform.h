#ifndef USERMENU_H
#define USERMENU_H

#include <Qtimer>
#include <QWidget>

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
		void slotButtonExit ();
		
		void slotReconnect ();
		void slotReceivedData (QByteArray data);
		
	private:
		Ui::UserForm* ui;
		Application* app;
		
		QTimer* reconnect;
		
		struct Machine {
			QString name;
			
			qreal health, armor;
			qreal speed, rotate;
			qreal damage, duration;
			qreal bullet, capacity;
		};
		
		Machine machine;
		
};

#endif // USERMENU_H
