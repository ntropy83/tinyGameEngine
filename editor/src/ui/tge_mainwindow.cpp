#include <QCloseEvent>
#include <QTextEdit>

#include "ui/tge_mainwindow.h"
#include "ui_mainwindow.h"

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
}
