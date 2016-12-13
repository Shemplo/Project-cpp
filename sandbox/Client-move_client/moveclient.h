#ifndef MOVECLIENT_H
#define MOVECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

class MoveClient : public QObject {
		
	Q_OBJECT
		
	public:
		MoveClient (QObject* parent = 0);
		
	public slots:
		void slotReadStream ();
		void slotDisconnected ();
		
	private:
		QTcpSocket* socket;
		qint32 x, y, areaWidth, areaHeight;
		
};

#endif // MOVECLIENT_H
