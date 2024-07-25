#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;

    mainWindow.setWindowTitle("SnappyTrim");
    mainWindow.show();

    return app.exec();
}