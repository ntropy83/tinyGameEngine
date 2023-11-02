#include "debug/tge_vulDebug.hpp"
#include "debug/tge_QtDebug.hpp"

#include <QApplication>
#include <QTimer>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
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

    QTreeView* treeView = w.findChild<QTreeView*>("treeView");
    QFileSystemModel fileSystemModel;
    fileSystemModel.setRootPath(QDir::rootPath());
    treeView->setModel(&fileSystemModel);
    treeView->setRootIndex(fileSystemModel.index(QDir::currentPath()));

    QWidget* glfwPlaceholder = w.findChild<QWidget*>("glfwPlaceholder");
    if (!glfwPlaceholder) {
        qCritical() << "Failed to find glfwPlaceholder";
        return EXIT_FAILURE;
    }

    // Integrate Vulkan window into the placeholder widget
    vulkan_window.integrateInto(glfwPlaceholder);

    w.show();

    QTextEdit* debugConsole = w.findChild<QTextEdit*>("debugConsole");
    tge::TgeDebug debugInstance;
    debugInstance.setTextEdit(debugConsole);
    qInstallMessageHandler(tge::TgeDebug::staticMessageHandler);

    // Flush any debug messages that might have been stored before the GUI was up
    tge::TgeVulDebug vulDebug;
    QString debugOutput = QString::fromStdString(vulDebug.flushBuffer());
    debugConsole->setHtml(debugOutput);

    auto runBoth = [&]() {
        QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            if (!vulkan_window.shouldClose()) {
                app.update();  
            } else {
                a.quit();
            }
        });
        timer.start(33);  // Approx. 30fps, formula is 1000 / wanted fps

        return a.exec();
    };

    int exitCode = EXIT_SUCCESS;
    try {
        exitCode = runBoth();
    } catch (const std::exception &e) {
        qCritical() << "An exception occurred: " << e.what();
        exitCode = EXIT_FAILURE;
    }

    FileSystem::Shutdown();
    return exitCode;
}
