#include <QCloseEvent>
#include <QTextEdit>

#include "ui/tge_mainwindow.h"
#include "qt_ui/ui_mainwindow.h"
#include "vulkan/tge_window.hpp"

MainWindow::MainWindow(tge::TgeWindow &tgeWindow, QWidget *parent)
    : QMainWindow(parent),  
    window_(tgeWindow), 
    ui(new Ui::MainWindow),
    isDocked(true)
{   
    ui->setupUi(this);

    connect(ui->actionDock, &QAction::triggered, this, &MainWindow::toggleDocking);
}

MainWindow::~MainWindow()
{
    // Connect the Quit action to the slot
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));

    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::toggleDocking() {
    if (isDocked) {
        // Undock the GLFW window
        window_.undockFrom(this); // You need to implement this method in TgeWindow
        isDocked = false;
    } else {
        // Dock the GLFW window
        window_.integrateInto(ui->VulkanView_Widget, ui->VulkanView_Widget->currentIndex()); // You need to implement this method in TgeWindow
        isDocked = true;
    }
}
