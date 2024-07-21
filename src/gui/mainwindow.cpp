#include "mainwindow.h"
#include "forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    m_Ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}