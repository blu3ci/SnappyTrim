#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaMetaData>

#include "mainwindow.h"
#include "forms/ui_mainwindow.h"
#include "../core/videotrimmer.h"
#include "widgets/videoplayer.h"
#include "widgets/importmediabutton.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), m_Ui(new Ui::MainWindow)
{
	m_Ui->setupUi(this);
	toggleTrimWidgets();

	connect(m_Ui->actionPlayOrPause, &QAction::triggered, m_Ui->mediaWidget, &VideoPlayer::playOrPausePlayer);
	connect(m_Ui->actionNext_Frame, &QAction::triggered, m_Ui->mediaWidget, &VideoPlayer::movePlayerOneFrameForward);
	connect(m_Ui->actionPrevious_Frame, &QAction::triggered, m_Ui->mediaWidget, &VideoPlayer::movePlayerOneFrameBackward);
	connect(m_Ui->actionSeekForward, &QAction::triggered, this, [this]()
			{ m_Ui->mediaWidget->seekPlayer(5000); });
	connect(m_Ui->actionSeekBackward, &QAction::triggered, this, [this]()
			{ m_Ui->mediaWidget->seekPlayer(-5000); });
	connect(m_Ui->importMediaButton, &QPushButton::clicked, this, &MainWindow::importMedia);
	connect(m_Ui->importMediaButton, &ImportMediaButton::fileDropped, this, &MainWindow::importMediaFromPath);
	connect(m_Ui->setStartTimeButton, &QPushButton::clicked, this, &MainWindow::setStartTimestamp);
	connect(m_Ui->setEndTimeButton, &QPushButton::clicked, this, &MainWindow::setEndTimestamp);
	connect(m_Ui->trimButton, &QPushButton::clicked, this, &MainWindow::saveTrimmedVideo);
	connect(m_Ui->mediaWidget->getPlayer(), &QMediaPlayer::durationChanged, this, [this](qint64 duration)
			{
			m_Ui->endTimeLabel->setText(m_Ui->mediaWidget->toTimestampFormat(duration));
			m_EndTimestamp = duration;
			calculateTrimLength(); });
}

MainWindow::~MainWindow()
{
	delete m_Ui;
}

void MainWindow::importMedia()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open file", QString(), "Media Files (" + m_FileFilter + ")");

	if (filePath.isEmpty())
		return;

	importMediaFromPath(QUrl::fromLocalFile(filePath));
}

void MainWindow::importMediaFromPath(const QUrl &source)
{
	if (!QDir::match(m_FileFilter, source.fileName()))
	{
		QMessageBox::warning(this, "Error", "Invalid Media File: " + source.fileName());
		return;
	}

	m_ImportedFilePath = source.toLocalFile();

	m_Ui->mediaWidget->setMedia(source);

	toggleTrimWidgets();
	resetTimestamps();
	updateWindowTitle();
}

void MainWindow::setStartTimestamp()
{
	qint64 startTimestamp = m_Ui->mediaWidget->getPlayer()->position();

	if (m_EndTimestamp != -1 && m_EndTimestamp <= startTimestamp)
	{
		m_Ui->statusbar->showMessage("Enter a valid start timestamp!", 5000);
		return;
	}

	m_StartTimestamp = startTimestamp;
	m_Ui->startTimeLabel->setText(m_Ui->mediaWidget->toTimestampFormat(startTimestamp));

	calculateTrimLength();
}

void MainWindow::setEndTimestamp()
{
	qint64 endTimestamp = m_Ui->mediaWidget->getPlayer()->position();

	if (m_StartTimestamp != -1 && m_StartTimestamp >= endTimestamp)
	{
		m_Ui->statusbar->showMessage("Enter a valid end timestamp!", 5000);
		return;
	}

	m_Ui->endTimeLabel->setText(m_Ui->mediaWidget->toTimestampFormat(endTimestamp));
	m_EndTimestamp = endTimestamp;

	calculateTrimLength();
}

void MainWindow::saveTrimmedVideo()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save File", QString(), "Media Files (" + m_FileFilter + ")");

	if (filePath.isEmpty())
		return;

	bool success = trimVideo(m_ImportedFilePath, filePath, static_cast<double>(m_StartTimestamp), static_cast<double>(m_EndTimestamp));

	if (!success)
	{
		QMessageBox::warning(this, "Error", "Could not trim video file.");
	}
}

void MainWindow::toggleTrimWidgets()
{
	bool isDisabled = m_ImportedFilePath.isEmpty();
	m_Ui->timestampOptionsFrame->setDisabled(isDisabled);
	m_Ui->menuPlayback->setDisabled(isDisabled);
}

void MainWindow::resetTimestamps()
{
	m_Ui->startTimeLabel->setText("00:00");
	m_Ui->endTimeLabel->setText("00:00");

	m_StartTimestamp = -1;
	m_EndTimestamp = -1;

	calculateTrimLength();
}

void MainWindow::calculateTrimLength()
{
	static QString defaultText = m_Ui->trimButton->text();

	if (m_StartTimestamp == -1 && m_EndTimestamp == -1)
	{
		m_Ui->trimButton->setText(defaultText);
		return;
	}

	QString lenTimestamp = m_Ui->mediaWidget->toTimestampFormat(m_EndTimestamp - m_StartTimestamp);
	m_Ui->trimButton->setText(defaultText + " (" + lenTimestamp + ")");

	m_Ui->mediaWidget->videoProgressBar->paintMarkers(m_StartTimestamp, m_EndTimestamp);
}

void MainWindow::updateWindowTitle()
{
	static QString appName = windowTitle();

	setWindowTitle(appName + " - " + m_ImportedFilePath);
}
