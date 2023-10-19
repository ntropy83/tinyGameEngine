#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vulkan/tge_window.hpp" // Include the GLFWWidget header

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        tge::TgeWindow *m_glfwWidget;  // Declare the GLFWWidget pointer
    };
    
#endif // MAINWINDOW_H
