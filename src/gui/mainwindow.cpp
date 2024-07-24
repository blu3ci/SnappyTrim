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

    m_Ui->statusbar->showMessage(filePath);
    m_Ui->mediaWidget->setMedia(QUrl::fromLocalFile(filePath));
}

void MainWindow::toggleTrimWidgets()
{
    bool disable = m_ImportedFilePath.isEmpty() ? true : false;

    m_Ui->startInput->setDisabled(disable);
    m_Ui->endInput->setDisabled(disable);
    m_Ui->trimButton->setDisabled(disable);
}