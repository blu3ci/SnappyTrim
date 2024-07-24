#include <QApplication>

#include "gui/mainwindow.h"
#include "gui/widgets/videoplayer.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    // auto player = new QMediaPlayer;
    // player->setSource(QUrl::fromLocalFile("C:/Users/madad/Downloads/make_him_race_the_car_behind_him_with_his_scoter_f018e4.mp4"));

    // auto videoWidget = new QVideoWidget;
    // player->setVideoOutput(videoWidget);

    // videoWidget->show();
    // player->play();

    // VideoPlayer *videoplayer = new VideoPlayer;
    // videoplayer->show();

    return app.exec();
}