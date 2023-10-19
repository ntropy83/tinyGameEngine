#include "vulkan/tge_window.hpp"

// include std
#include <stdexcept>
namespace tge {

  TgeWindow::TgeWindow(int w, int h, std::string name, QWidget *parent) : width{w}, height{h}, windowName{name} {
    initWindow();
  }

  TgeWindow::~TgeWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void TgeWindow::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  }
    
  void TgeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface");
    }
  }

} // namespace tge

