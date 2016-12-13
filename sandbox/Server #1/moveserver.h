#ifndef MOVESERVER_H
#define MOVESERVER_H

#include <QTimer>
#include <QObject>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>

class ServerConnection;

class MoveServer : public QObject {
		
	Q_OBJECT
		
	public:
		explicit MoveServer (QObject* parent = 0);
		qint32 activeSockets ();
		QVector <std::pair <qint32, qint32>>* positions ();
		void disconnectSocket (ServerConnection* socket);
		qint32 areaWidth, areaHeight;
		
	signals:
		void signalUpdate ();
		
	public slots:
		void slotNewConnection ();
		
	private slots:
		void slotUpdate ();
		
	private:
		QTcpServer* server;
		QVector <ServerConnection*> connects;
		QVector <std::pair <qint32, qint32>>* coords;
		
};

#endif // MOVESERVER_H
