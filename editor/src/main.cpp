#include "tge_editor.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  FileSystem::Init();
  FileSystem::Mount("data/data.zip", "/");
  
  tge::TgeEditor app{};

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  FileSystem::Shutdown();
  return EXIT_SUCCESS;
}