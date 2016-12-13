#include <QCoreApplication>
#include "moveclient.h"

int main (int argc, char *argv[]) {
	QCoreApplication a (argc, argv);
	MoveClient client;
	return a.exec ();
}
