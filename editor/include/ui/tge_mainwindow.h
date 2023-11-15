#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vulkan/tge_window.hpp"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(tge::TgeWindow &tgeWindow, QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    private slots:
        void on_actionQuit_triggered();
        void toggleDocking();     

    private:
        Ui::MainWindow *ui;
        tge::TgeWindow &window_;
        bool isDocked;
    };

#endif // MAINWINDOW_H
