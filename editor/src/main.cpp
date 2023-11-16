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
#include <qtabwidget.h>
#include <stdexcept>
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {
    FileSystem::Init();
    #ifdef _WIN32
        FileSystem::Mount("data.zip", "/");
    #else
        FileSystem::Mount("data.zip", "/");
    #endif

    QApplication a(argc, argv);

    TgeSplashScreen splash;
    splash.setWindowFlags(Qt::FramelessWindowHint);
    splash.show();

    // Allow UI to update and show the splash screen
    a.processEvents();

    // Start loading Vulkan and other resources here.
    // Placeholder for actual loading code:
    std::this_thread::sleep_for(std::chrono::seconds(0));  // Simulate some loading

    // After the loading is done or after 1 second, proceed to create the main window
    tge::TgeEditor app{}; 
    tge::TgeWindow& vulkan_window = app.getWindow();
    MainWindow w(vulkan_window);

    // Setup tree view
    QTreeView* assets_browser = w.findChild<QTreeView*>("treeView");
    if (!assets_browser) {
        qCritical() << "Failed to find treeView(assets_browser)";
        splash.close();
        return EXIT_FAILURE;
    }

    QFileSystemModel fileSystemModel;
    fileSystemModel.setRootPath(QDir::rootPath());
    assets_browser->setModel(&fileSystemModel);
    assets_browser->setRootIndex(fileSystemModel.index(QDir::currentPath()));

    QTabWidget* vulkanViewWidget = w.findChild<QTabWidget*>("VulkanView_Widget");
    if (!vulkanViewWidget) {
        qCritical() << "Failed to find vulkanViewWidget";
        splash.close();
        return EXIT_FAILURE;
    }

    // Integrate Vulkan window into the placeholder widget
    vulkan_window.integrateInto(vulkanViewWidget, vulkanViewWidget->currentIndex());

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
    timer.start(32); // Approx. 30fps, formula is 1000 / wanted fps

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
