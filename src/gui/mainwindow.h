#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void importMedia();
    void importMediaFromPath(const QUrl &source);
    void setStartTimestamp();
    void setEndTimestamp();
    void saveTrimmedVideo();

private:
    Ui::MainWindow *m_Ui;
    QString m_ImportedFilePath;
    qint64 m_StartTimestamp = -1;
    qint64 m_EndTimestamp = -1;
    QString m_FileFilter = "*.mp4 *.mov *.webm";

    void toggleTrimWidgets();
    void resetTimestamps();
    void calculateTrimLength();
    void updateWindowTitle();
};