#include <QPainter>

#include "videoprogressbar.h"
#include "videoplayer.h"

VideoProgressBar::VideoProgressBar(Qt::Orientation orientation, QMediaPlayer* mediaPlayer, QWidget* parent)
	: QWidget(parent), m_MediaPlayer(mediaPlayer)
{
	m_Layout = new QHBoxLayout;

	setLayout(m_Layout);

	m_VideoProgressBar = new QSlider(orientation);
	m_ProgressLabel = new QLabel("00:00 / 00:00");

	m_Layout->addWidget(m_VideoProgressBar);
	m_Layout->addWidget(m_ProgressLabel);

	connect(m_VideoProgressBar, &QSlider::sliderPressed, this, [this]()
		{
			if (m_MediaPlayer->isPlaying())
			{
				m_MediaPlayer->pause();
				m_SystemPaused = true;
			} });

			connect(m_VideoProgressBar, &QSlider::sliderReleased, this, &VideoProgressBar::setPlayerPosition);
			connect(m_MediaPlayer, &QMediaPlayer::durationChanged, this, &VideoProgressBar::setProgressBarDuration);
			connect(m_MediaPlayer, &QMediaPlayer::positionChanged, this, &VideoProgressBar::setProgressBarPosition);
}

void VideoProgressBar::paintMarkers(qint64 marker1Ms, qint64 marker2Ms)
{
	m_Marker1Ms = marker1Ms;
	m_Marker2Ms = marker2Ms;
	repaint();
}

void VideoProgressBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setBrush(Qt::green);
	painter.setPen(Qt::black);

	QSizeF markerSize(10, 10);
	qreal sliderMaxXValue = m_VideoProgressBar->width() - markerSize.height();

	qreal marker1Percentage = static_cast<qreal>(m_Marker1Ms) / m_MediaPlayer->duration();
	qreal marker2Percentage = static_cast<qreal>(m_Marker2Ms) / m_MediaPlayer->duration();

	QPointF marker1Pos(sliderMaxXValue * marker1Percentage + markerSize.height() * (1.0 - marker1Percentage), 0);
	QPointF marker2Pos(sliderMaxXValue * marker2Percentage + markerSize.height() * (1.0 - marker2Percentage), 0);

	drawTriangle(&painter, marker1Pos, markerSize);
	drawTriangle(&painter, marker2Pos, markerSize, true);
}

void VideoProgressBar::setPlayerPosition()
{
	m_MediaPlayer->setPosition(m_VideoProgressBar->value());

	if (m_SystemPaused)
	{
		m_MediaPlayer->play();
		m_SystemPaused = false;
	}
}

void VideoProgressBar::setProgressBarDuration(qint64 position)
{
	m_VideoProgressBar->setMinimum(0);
	m_VideoProgressBar->setMaximum(position);
}

void VideoProgressBar::setProgressBarPosition(qint64 position)
{
	m_VideoProgressBar->setValue(position);

	m_ProgressLabel->setText(VideoPlayer::toTimestampFormat(position) +
		" / " + VideoPlayer::toTimestampFormat(m_MediaPlayer->duration()));
}

void VideoProgressBar::drawTriangle(QPainter* painter, const QPointF& topLeft, const QSizeF& size, bool flip)
{
	QPointF points[3] = {
		QPointF(topLeft),
		QPointF(topLeft + QPointF(0, size.height())),
		QPointF(topLeft + QPointF(size.width(), (topLeft.y() + size.height()) / 2))
	};

	if (flip)
	{
		points[0] += QPointF(size.width(), 0);
		points[1] += QPointF(size.width(), 0);
		points[2] += QPointF(-size.width(), 0);
	}

	painter->drawPolygon(points, 3);
}
