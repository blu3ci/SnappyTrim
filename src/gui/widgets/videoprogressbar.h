#pragma once

#include <QSlider>
#include <QMediaPlayer>
#include <QLabel>
#include <QHBoxLayout>

class VideoProgressBar : public QWidget
{
	Q_OBJECT

public:

	VideoProgressBar(Qt::Orientation orientation, QMediaPlayer* mediaPlayer, QWidget* parent = nullptr);

	void paintMarkers(qint64 marker1Ms, qint64 marker2Ms);

protected:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void setPlayerPosition();
	void setProgressBarDuration(qint64 duration);
	void setProgressBarPosition(qint64 position);

private:
	bool m_SystemPaused = false;
	QHBoxLayout* m_Layout;
	QMediaPlayer* m_MediaPlayer;
	QSlider* m_VideoProgressBar;
	QLabel* m_ProgressLabel;
	qint64 m_Marker1Ms = 0;
	qint64 m_Marker2Ms = 0;
};