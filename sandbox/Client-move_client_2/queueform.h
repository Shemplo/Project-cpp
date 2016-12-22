#ifndef QUEUEFORM_H
#define QUEUEFORM_H

#include <QTimer>
#include <QWidget>

#include <application.h>

namespace Ui {
	class QueueForm;
}

class QueueForm : public QWidget {
		
	Q_OBJECT
		
	public:
		explicit QueueForm (Application* app);
		~QueueForm ();
		
		void setDefault ();
		void updateData (QString target);
		
	signals:
		void signalSwitchForm (int form);
		
	public slots:
		void slotDisconnected ();
		
	private slots:
		void slotReceivedData (QByteArray data);
		void slotPlusSecond   ();
		
		void slotButtonLeave  ();
		
	private:
		Ui::QueueForm* ui;
		Application* app;
		
		QTimer* time;
		qint64 timeIn = 0;
		
		qint32 users = 0;
		qint32 id = 0, level;
		QString machineName;
		
};

#endif // QUEUEFORM_H
