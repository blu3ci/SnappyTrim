#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(snappytrim);

	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}