#include "debug/tge_vulDebug.hpp"
#include "debug/tge_QtDebug.hpp"

#include <QApplication>
#include <QTimer>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
#include "ui/tge_vulkanwindow.hpp"
#include "ui/tge_mainwindow.h"
#include "ui/tge_splashscreen.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {
    FileSystem::Init();
    FileSystem::Mount("data.zip", "/");

    QApplication a(argc, argv);

    TgeSplashScreen splash;
    splash.setWindowFlags(Qt::FramelessWindowHint);
    splash.show();

    // Allow UI to update and show the splash screen
    a.processEvents();

    // Start loading Vulkan and other resources here.
    // Placeholder for actual loading code:
    std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate some loading

    // After the loading is done or after 1 second, proceed to create the main window
    tge::TgeEditor app{}; 
    tge::TgeWindow& vulkan_window = app.getWindow();
    MainWindow w(vulkan_window);

    // Setup tree view
    QTreeView* treeView = w.findChild<QTreeView*>("treeView");
    QFileSystemModel fileSystemModel;
    fileSystemModel.setRootPath(QDir::rootPath());
    treeView->setModel(&fileSystemModel);
    treeView->setRootIndex(fileSystemModel.index(QDir::currentPath()));

    // Setup GLFW placeholder
    QWidget* glfwPlaceholder = w.findChild<QWidget*>("glfwPlaceholder");
    if (!glfwPlaceholder) {
        qCritical() << "Failed to find glfwPlaceholder";
        splash.close();
        return EXIT_FAILURE;
    }

    // Integrate Vulkan window into the placeholder widget
    vulkan_window.integrateInto(glfwPlaceholder);

    // Setup debug console
    QTextEdit* debugConsole = w.findChild<QTextEdit*>("debugConsole");
    tge::TgeDebug debugInstance;
    debugInstance.setTextEdit(debugConsole);
    qInstallMessageHandler(tge::TgeDebug::staticMessageHandler);

    // Flush any debug messages that might have been stored before the GUI was up
    tge::TgeVulDebug vulDebug;
    QString debugOutput = QString::fromStdString(vulDebug.flushBuffer());
    debugConsole->setHtml(debugOutput);

    // Show the main window and close the splash screen
    w.show();
    splash.close();

    // Connect the update of Vulkan window to the application event loop
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (!vulkan_window.shouldClose()) {
            app.update();  
        } else {
            a.quit();
        }
    });
    timer.start(33); // Approx. 30fps, formula is 1000 / wanted fps

    // Run the application event loop
    int exitCode = EXIT_SUCCESS;
    try {
        exitCode = a.exec();
    } catch (const std::exception &e) {
        qCritical() << "An exception occurred: " << e.what();
        exitCode = EXIT_FAILURE;
    }

    FileSystem::Shutdown();
    return exitCode;
}
