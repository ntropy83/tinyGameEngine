// tge_splashscreen.cpp
#include "debug/tge_QtDebug.hpp"

#include "ui/tge_splashscreen.h"
#include "qt_ui/ui_tge_splashscreen.h"

TgeSplashScreen::TgeSplashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TgeSplashScreen)
{
    QPixmap pixmap("res/splash.png");
    if (pixmap.isNull()) {
        qDebug() << "Failed to load splash image";
    }
    label = new QLabel(this); // initialize label
    label->setPixmap(QPixmap(":/splash.png"));

    ui->setupUi(this);
}

TgeSplashScreen::~TgeSplashScreen()
{
    delete ui;
}

void TgeSplashScreen::setProgress(int value)
{
    if (ui->progressBar) { // Now accessing progressBar through the ui pointer
        ui->progressBar->setValue(value);
    }
}
