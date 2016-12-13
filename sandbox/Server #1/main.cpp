#include <QCoreApplication>
#include "moveserver.h"

int main (int argc, char* argv []) {
	QCoreApplication a (argc, argv);
	MoveServer server;
	return a.exec ();
}
