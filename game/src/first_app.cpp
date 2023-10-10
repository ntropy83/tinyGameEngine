#include "first_app.hpp"

#include "vulkan/tge_pipeline_config.hpp"

namespace tge {

  void FirstApp::run() {

    while (!tgeWindow.shouldClose()) {
      glfwPollEvents();
    }
  };

} // namespace tge