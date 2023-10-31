#include "vulkan/tge_window.hpp"

#include "debug/tge_vulDebug.hpp"

#include "GLFW/glfw3.h"

// include std
#include <stdexcept>
#include <iostream>

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
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

    int xpos = 0;
    int ypos = 0;
    glfwSetWindowPos(window, xpos, ypos);
  }
    
  void TgeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {

      TgeVulDebug vulDebug;
      vulDebug.writeToBuffer("failed to create window surface");

      throw std::runtime_error("failed to create window surface");
    }
  }

  void TgeWindow::integrateInto(QWidget* placeholder){
      Window glfwNativeWindow = glfwGetX11Window(window);
      Window qtNativeWindow = placeholder->winId();

      Display* display = glfwGetX11Display();
      XReparentWindow(display, glfwNativeWindow, qtNativeWindow, 0, 0);

      glfwShowWindow(window);
  }
} // namespace tge

