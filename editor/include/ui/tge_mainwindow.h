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
        void changeEvent(QEvent* event) override;
        void closeEvent(QCloseEvent *event) override;
        void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    private:
        Ui::MainWindow *ui;
        tge::TgeWindow &window_;
    };

#endif // MAINWINDOW_H
