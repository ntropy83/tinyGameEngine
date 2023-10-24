#include "debug/tge_QtDebug.hpp"
#include <QApplication>
#include <QTimer>

#include "ui/tge_vulkanwindow.hpp"
#include "ui/tge_mainwindow.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {

    FileSystem::Init();
    FileSystem::Mount("data.zip", "/");

    QApplication a(argc, argv);
    tge::TgeEditor app{}; 
    tge::TgeWindow& vulkan_window = app.getWindow();  

    MainWindow w(vulkan_window);
    w.show();

    tge::TgeDebug debugInstance;
    debugInstance.setTextEdit(w.findChild<QTextEdit*>("textEdit"));

    qInstallMessageHandler(tge::TgeDebug::staticMessageHandler); 

    qDebug() << "\nDevice count: 4\nphysical device: AMD Radeon Graphics (RADV REMBRANDT)\nPresent mode: V-Sync \nglslang version: 4.60 glslang Khronos. 13.0.0\nCompiling vertex shader: shaders/simple_shader.vert\nCompiling fragment shader: shaders/simple_shader.frag\n";

    auto runBoth = [&]() {
        QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            if (!vulkan_window.shouldClose()) {
                app.update();  
            }else{
                a.quit();
            }
        });
        timer.start(33);  // Check every 33ms (approx. 30fps), formula: 1000/fps = ms

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