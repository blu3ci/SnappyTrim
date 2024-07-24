#pragma once

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>

class VideoPlayer : public QWidget
{
public:
    VideoPlayer(QWidget *parent = nullptr);

    void setMedia(const QUrl &source);

private:
    QVBoxLayout *m_Layout;
    QVideoWidget *m_VideoWidget;
    QMediaPlayer *m_MediaPlayer;
    QAudioOutput *m_AudioOutput;

    void configureMediaControls();
};