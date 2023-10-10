#include "first_app.hpp"

namespace tge {

  void FirstApp::run() {

    while (!tgeWindow.shouldClose()) {
      glfwPollEvents();
    }
  };

} // namespace tge