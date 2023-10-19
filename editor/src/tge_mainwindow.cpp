#include "tge_mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_glfwWidget = new tge::TgeWindow(1280, 800, "Hello Vulkan", this);  // Initialize the GLFW widget
    setCentralWidget(m_glfwWidget);  // Set it as the central widget of the main window
}

MainWindow::~MainWindow()
{
    delete ui;
}