#include <QCloseEvent>

#include "ui/tge_mainwindow.h"
#include "ui_mainwindow.h"
#include "vulkan/tge_window.hpp"

MainWindow::MainWindow(tge::TgeWindow &tgeWindow, QWidget *parent)
    : QMainWindow(parent), 
    window_(tgeWindow), 
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent* event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            #ifdef __linux__
            window_.minimizeGLFWWindow(window_.window);
            #endif
        } else if (isMaximized()) {
            #ifdef __linux__
            window_.maximizeGLFWWindow(window_.window);
            #endif
        }
        if (windowState() & Qt::WindowMaximized) {
            window_.restoreGLFWWindow(window_.window);
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Do any necessary cleanup related to your TgeWindow or other components here

    event->accept();  // Continue with the regular close process
}