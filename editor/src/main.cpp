#include <QApplication>
#include <QTimer>

#include "tge_vulkanwindow.hpp"
#include "tge_mainwindow.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {
    FileSystem::Init();
    FileSystem::Mount("data.zip", "/");

    QApplication a(argc, argv);  

    MainWindow w;
    w.show();

    tge::TgeEditor app{}; 
    tge::TgeWindow& vulkan_window = app.getWindow();

    auto runBoth = [&]() {
        QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            if (!vulkan_window.shouldClose()) {
                app.update();  
            }else{
                a.quit();
            }
        });
        timer.start(33);  // Check every 16ms (approx. 30fps), formula: 1000/fps = ms

        return a.exec();
    };

    try {
        return runBoth();
    } catch (const std::exception &e) {
        FileSystem::Shutdown();
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    FileSystem::Shutdown();
    return EXIT_SUCCESS;
}