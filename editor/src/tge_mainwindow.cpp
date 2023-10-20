#include <QCloseEvent>

#include "tge_mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Do any necessary cleanup related to your TgeWindow or other components here

    event->accept();  // Continue with the regular close process
}