#include <QToolButton>
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

void VideoPlayer::configureMediaControls()
{
	m_Layout->addWidget(m_ControlsFrame);

	m_ControlsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	QVBoxLayout *mediaControls = new QVBoxLayout;
	QHBoxLayout *buttonLayout = new QHBoxLayout;

	m_ControlsFrame->setLayout(mediaControls);

	videoProgressBar = new VideoProgressBar(Qt::Horizontal, m_MediaPlayer);

	mediaControls->addWidget(videoProgressBar);

	mediaControls->addLayout(buttonLayout);

	QToolButton *playButton = new QToolButton();
	QToolButton *pauseButton = new QToolButton();

	playButton->setIcon(QIcon(":/res/icons/playIcon"));
	playButton->setIconSize({32, 32});

	pauseButton->setIcon(QIcon(":/res/icons/pauseIcon"));
	pauseButton->setIconSize({32, 32});

	buttonLayout->addWidget(playButton);
	buttonLayout->addWidget(pauseButton);
	buttonLayout->setAlignment(Qt::AlignLeft);

	connect(playButton, &QToolButton::clicked, this, &VideoPlayer::resumePlayer);
	connect(pauseButton, &QToolButton::clicked, this, &VideoPlayer::pausePlayer);
}

void VideoPlayer::toggleControlsFrame()
{
	m_ControlsFrame->setDisabled(m_Source.isEmpty());
}
