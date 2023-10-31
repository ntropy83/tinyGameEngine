#include "debug/tge_vulDebug.hpp"
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


    QWidget* glfwPlaceholder = w.findChild<QWidget*>("glfwPlaceholder");
    if (!glfwPlaceholder) {
        qCritical() << "Failed to find glfwPlaceholder";
        return EXIT_FAILURE;
    }

    // Integrate Vulkan window into the placeholder widget
    vulkan_window.integrateInto(glfwPlaceholder);

    w.show();

    tge::TgeDebug debugInstance;
    debugInstance.setTextEdit(w.findChild<QTextEdit*>("debugConsole"));
    qInstallMessageHandler(tge::TgeDebug::staticMessageHandler); 

    tge::TgeVulDebug vulDebug;
    vulDebug.writeToBuffer("Test Vulkan Debug Message");
    std::string debugOutput = vulDebug.flushBuffer();
    qDebug() << QString::fromStdString(debugOutput);

    auto runBoth = [&]() {
        QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            if (!vulkan_window.shouldClose()) {
                app.update();  
            }else{
                a.quit();
            }
        });
        timer.start(33);  // Approx. 30fps, formula is 1000 / wanted fps

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
