#include <QCloseEvent>
#include <QTextEdit>

#include "ui/tge_mainwindow.h"
#include "qt_ui/ui_mainwindow.h"

MainWindow::MainWindow(tge::TgeWindow &tgeWindow, QWidget *parent)
    : QMainWindow(parent),  
    window_(tgeWindow), 
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    //QTextEdit *textEditConsole = this->ui->textEdit;
}

MainWindow::~MainWindow()
{
    delete ui;

    // Connect the Quit action to the slot
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

