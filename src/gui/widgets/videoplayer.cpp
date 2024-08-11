#include <QPushButton>
#include <QHBoxLayout>
#include <QTime>
#include <QLabel>
#include <QMediaMetaData>

#include "videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent), m_Layout(new QVBoxLayout(this)),
      m_VideoWidget(new QVideoWidget), m_MediaPlayer(new QMediaPlayer),
      m_AudioOutput(new QAudioOutput), m_ControlsFrame(new QFrame)
{
    m_Layout->addWidget(m_VideoWidget);
    setLayout(m_Layout);

    m_MediaPlayer->setVideoOutput(m_VideoWidget);
    m_MediaPlayer->setAudioOutput(m_AudioOutput);
    m_AudioOutput->setVolume(50);

    configureMediaControls();
    toggleControlsFrame();

    connect(m_MediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::setProgressBarDuration);
    connect(m_MediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::setProgressBarPosition);
}

QString VideoPlayer::toTimestampFormat(qint64 timeMs)
{
    int seconds = timeMs / 1000;

    int minutes = seconds / 60;
    seconds %= 60;

    int hours = minutes / 60;
    minutes %= 60;

    QTime time(hours, minutes, seconds);

    QString format = timeMs >= (60 * 60 * 1000) ? "hh:mm:ss" : "mm:ss";

    return time.toString(format);
}

void VideoPlayer::setMedia(const QUrl &source)
{
    if (m_MediaPlayer->isPlaying())
        m_MediaPlayer->stop();

    m_MediaPlayer->setSource(source);
    m_MediaPlayer->play();

    m_Source = source.toString();

    toggleControlsFrame();
}

const QMediaPlayer *VideoPlayer::getPlayer() const
{
    return m_MediaPlayer;
}

void VideoPlayer::pausePlayer()
{
    if (m_MediaPlayer->isPlaying())
        m_MediaPlayer->pause();
}

void VideoPlayer::resumePlayer()
{
    if (!m_MediaPlayer->isPlaying())
        m_MediaPlayer->play();
}

void VideoPlayer::playOrPausePlayer()
{
    if (m_MediaPlayer->isPlaying())
    {
        pausePlayer();
    }
    else
    {
        resumePlayer();
    }
}

void VideoPlayer::seekPlayer(qint64 position)
{
    qint64 absolutePos = m_MediaPlayer->position() + position;

    if (absolutePos >= m_MediaPlayer->duration())
    {
        m_MediaPlayer->setPosition(m_MediaPlayer->duration());
    }
    else
    {
        m_MediaPlayer->setPosition(absolutePos);
    }
}

void VideoPlayer::movePlayerOneFrameForward()
{
    if (!m_MediaPlayer->isPlaying())
        seekPlayer(m_MediaPlayer->metaData()[QMediaMetaData::VideoFrameRate].toInt());
}

void VideoPlayer::movePlayerOneFrameBackward()
{
    if (!m_MediaPlayer->isPlaying())
        seekPlayer(-m_MediaPlayer->metaData()[QMediaMetaData::VideoFrameRate].toInt());
}

void VideoPlayer::setProgressBarDuration(qint64 position)
{
    m_VideoProgressBar->setMinimum(0);
    m_VideoProgressBar->setMaximum(position);
}

void VideoPlayer::setProgressBarPosition(qint64 position)
{
    m_VideoProgressBar->setValue(position);

    m_ProgressLabel->setText(VideoPlayer::toTimestampFormat(position) +
                             " / " + VideoPlayer::toTimestampFormat(m_MediaPlayer->duration()));
}

void VideoPlayer::setPlayerPosition()
{
    m_MediaPlayer->setPosition(m_VideoProgressBar->value());

    if (m_SystemPaused == true)
    {
        m_MediaPlayer->play();
        m_SystemPaused = false;
    }
}

void VideoPlayer::configureMediaControls()
{
    m_Layout->addWidget(m_ControlsFrame);

    m_ControlsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QVBoxLayout *mediaControls = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QHBoxLayout *progressBarLayout = new QHBoxLayout;

    m_ControlsFrame->setLayout(mediaControls);

    mediaControls->addLayout(progressBarLayout);

    m_VideoProgressBar = new QSlider(Qt::Horizontal);
    m_ProgressLabel = new QLabel("00:00 / 00:00");

    progressBarLayout->addWidget(m_VideoProgressBar);
    progressBarLayout->addWidget(m_ProgressLabel);

    connect(m_VideoProgressBar, &QSlider::sliderPressed, this, [this]()
            {
                if (m_MediaPlayer->isPlaying())
                {
                    m_MediaPlayer->pause();
                    m_SystemPaused = true;
                } });

    connect(m_VideoProgressBar, &QSlider::sliderReleased, this, &VideoPlayer::setPlayerPosition);

    mediaControls->addLayout(buttonLayout);

    QPushButton *playButton = new QPushButton("Play");
    QPushButton *pauseButton = new QPushButton("Pause");

    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(pauseButton);

    connect(playButton, &QPushButton::clicked, this, &VideoPlayer::resumePlayer);
    connect(pauseButton, &QPushButton::clicked, this, &VideoPlayer::pausePlayer);
}

void VideoPlayer::toggleControlsFrame()
{
    m_ControlsFrame->setDisabled(m_Source.isEmpty());
}
