#include <QPushButton>
#include <QHBoxLayout>
#include <QProgressBar>

#include "videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent), m_Layout(new QVBoxLayout(this)),
      m_VideoWidget(new QVideoWidget), m_MediaPlayer(new QMediaPlayer), m_AudioOutput(new QAudioOutput)
{
    m_Layout->addWidget(m_VideoWidget);
    setLayout(m_Layout);

    m_MediaPlayer->setVideoOutput(m_VideoWidget);
    m_MediaPlayer->setAudioOutput(m_AudioOutput);
    m_AudioOutput->setVolume(50);

    configureMediaControls();
}

void VideoPlayer::setMedia(const QUrl &source)
{
    if (m_MediaPlayer->isPlaying())
        m_MediaPlayer->stop();

    m_MediaPlayer->setSource(source);
    m_MediaPlayer->play();
}

void VideoPlayer::configureMediaControls()
{
    QVBoxLayout *mediaControls = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    m_Layout->addLayout(mediaControls);
    mediaControls->addLayout(buttonLayout);

    QPushButton *playButton = new QPushButton("Play");
    QPushButton *pauseButton = new QPushButton("Pause");

    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(pauseButton);

    QProgressBar *videoProgressBar = new QProgressBar;
    mediaControls->addWidget(videoProgressBar);
}
