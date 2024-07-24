#include "mainwindow.h"
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>

#include "mainwindow.h"
#include "forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_Ui(new Ui::MainWindow)
{
    m_Ui->setupUi(this);
    toggleTrimWidgets();

    connect(m_Ui->importMediaButton, &QPushButton::clicked, this, &MainWindow::importMedia);
    connect(m_Ui->setStartTimeButton, &QPushButton::clicked, this, &MainWindow::setStartTimestamp);
    connect(m_Ui->setEndTimeButton, &QPushButton::clicked, this, &MainWindow::setEndTimestamp);
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

void MainWindow::importMedia()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open file", QString(), "Media Files (*.mp4 *.mov *.webm)");

    if (filePath.isEmpty())
        return;

    m_ImportedFilePath = filePath;

    toggleTrimWidgets();
    resetTimestamps();

    m_Ui->mediaWidget->setMedia(QUrl::fromLocalFile(filePath));
}

void MainWindow::setStartTimestamp()
{
    qint64 startTimestamp = m_Ui->mediaWidget->getPlayer().position();

    if (m_EndTimestamp != -1 && m_EndTimestamp <= startTimestamp)
    {
        m_Ui->statusbar->showMessage("Enter a valid start timestamp!", 5000);
        return;
    }

    m_StartTimestamp = startTimestamp;
    m_Ui->startTimeLabel->setText(m_Ui->mediaWidget->toTimestampFormat(startTimestamp));
}

void MainWindow::setEndTimestamp()
{
    qint64 endTimestamp = m_Ui->mediaWidget->getPlayer().position();

    if (m_StartTimestamp != -1 && m_StartTimestamp >= endTimestamp)
    {
        m_Ui->statusbar->showMessage("Enter a valid end timestamp!", 5000);
        return;
    }

    m_Ui->endTimeLabel->setText(m_Ui->mediaWidget->toTimestampFormat(endTimestamp));
    m_EndTimestamp = endTimestamp;
}

void MainWindow::toggleTrimWidgets()
{
    m_Ui->timestampOptionsFrame->setDisabled(m_ImportedFilePath.isEmpty());
}

void MainWindow::resetTimestamps()
{
    m_Ui->startTimeLabel->setText("00:00 / 00:00");
    m_Ui->endTimeLabel->setText("00:00 / 00:00");

    m_StartTimestamp = -1;
    m_EndTimestamp = -1;
}
