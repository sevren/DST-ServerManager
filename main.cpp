#include "dstservermanager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	dstServerManager w;

	
	w.show();
	return a.exec();
}
