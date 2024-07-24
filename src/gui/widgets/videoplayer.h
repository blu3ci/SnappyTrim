#pragma once

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QFrame>
#include <QSlider>
#include <QLabel>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = nullptr);

    static QString toTimestampFormat(qint64 timeMs);

    void setMedia(const QUrl &source);
    const QMediaPlayer &getPlayer() const;

private slots:
    void setProgressBarDuration(qint64 duration);
    void setProgressBarPosition(qint64 position);
    void setPlayerPosition();

private:
    bool m_SystemPaused = false;
    QVBoxLayout *m_Layout;
    QVideoWidget *m_VideoWidget;
    QMediaPlayer *m_MediaPlayer;
    QAudioOutput *m_AudioOutput;
    QFrame *m_ControlsFrame;
    QString m_Source;
    QSlider *m_VideoProgressBar;
    QLabel *m_ProgressLabel;

    void configureMediaControls();
    void toggleControlsFrame();
};