#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <iostream>

#include <QTcpSocket>
#include <QMainWindow>

#include <userform.h>

namespace Ui {
	class Application;
}

class Application : public QMainWindow {
	
	Q_OBJECT
		
	public:
		explicit Application (QWidget *parent = 0);
		~Application ();
		
		void connectHost      (QString host, qint32 port);
		void disconnectHost   ();
		bool isConnected      ();
		void writeInSocket    (QByteArray data);
		
		void closeApplication ();
		
	signals:
		void signalReceivedData (QByteArray data);
		
	private slots:
		void slotConnected    ();
		void slotDisconnected ();
		void slotReadStream   ();
		void slotSwitchForm   (int form);
		
	private:
		Ui::Application* ui;
		
		QTcpSocket* socket;
		bool connected = false;
		
		qint32 need = 0;
		QByteArray bufferArray;
};

#endif // APPLICATIONWINDOW_H
