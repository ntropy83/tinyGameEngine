#ifndef TGESPLASHSCREEN_H
#define TGESPLASHSCREEN_H

#include <QWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class TgeSplashScreen; }
QT_END_NAMESPACE

class TgeSplashScreen : public QWidget {
    Q_OBJECT

public:
    explicit TgeSplashScreen(QWidget *parent = nullptr);
    ~TgeSplashScreen();

    void setProgress(int value);

private:
    QLabel *label;
    Ui::TgeSplashScreen *ui;
};

#endif // TGESPLASHSCREEN_H
