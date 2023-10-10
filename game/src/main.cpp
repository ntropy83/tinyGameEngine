#include "first_app.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  FileSystem::Init();
  FileSystem::Mount("build/debug/data/data.zip", "/");
  
  tge::FirstApp app{};

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  FileSystem::Shutdown();
  return EXIT_SUCCESS;
}