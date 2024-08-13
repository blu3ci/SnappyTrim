#pragma once

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

#include "videoprogressbar.h"

class VideoPlayer : public QWidget
{
	Q_OBJECT

public:
	VideoProgressBar* videoProgressBar;

	VideoPlayer(QWidget* parent = nullptr);

	static QString toTimestampFormat(qint64 timeMs);
	void setMedia(const QUrl& source);

	const QMediaPlayer* getPlayer() const;

public slots:
	void pausePlayer();
	void resumePlayer();
	void playOrPausePlayer();
	void seekPlayer(qint64 position);
	void movePlayerOneFrameForward();
	void movePlayerOneFrameBackward();

private:
	QVBoxLayout* m_Layout;
	QVideoWidget* m_VideoWidget;
	QMediaPlayer* m_MediaPlayer;
	QAudioOutput* m_AudioOutput;
	QFrame* m_ControlsFrame;
	QString m_Source;

	void configureMediaControls();
	void toggleControlsFrame();
};