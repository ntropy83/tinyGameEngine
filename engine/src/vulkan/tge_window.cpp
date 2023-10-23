#include "vulkan/tge_window.hpp"
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
      throw std::runtime_error("failed to create window surface");
    }
  }

  void TgeWindow::minimizeGLFWWindow(GLFWwindow* window) {
      #ifdef __linux__
        Display* display = glfwGetX11Display();
        Window x11Window = glfwGetX11Window(window);
        XIconifyWindow(display, x11Window, DefaultScreen(display));
      #endif

      #ifdef _WIN32
        HWND hwnd = glfwGetWin32Window(window);
        ShowWindow(hwnd, SW_MINIMIZE);
      #endif
  }

  void TgeWindow::maximizeGLFWWindow(GLFWwindow* window) {
      #ifdef __linux__
        glfwMaximizeWindow(window);
      #endif

      #ifdef _WIN32
        HWND hwnd = glfwGetWin32Window(window);
        ShowWindow(hwnd, SW_MAXIMIZE);
      #endif
  }

  void TgeWindow::restoreGLFWWindow(GLFWwindow* window) {
      #ifdef __linux__
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
          glfwRestoreWindow(window);
        }
        glfwShowWindow(window);
      #endif

      #ifdef _WIN32
        if (IsIconic(glfwGetWin32Window(window))) {
          glfwRestoreWindow(window);
        }
        glfwShowWindow(window);
      #endif
  }
} // namespace tge

