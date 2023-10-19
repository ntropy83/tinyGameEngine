#include <QApplication>

#include "tge_editor.hpp"
#include "tge_mainwindow.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {
  FileSystem::Init();
  FileSystem::Mount("data.zip", "/");

  QApplication a(argc, argv);  
  tge::TgeEditor app{};

  MainWindow w;
  
  w.show();
  return a.exec();

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  FileSystem::Shutdown();
  return EXIT_SUCCESS;
}