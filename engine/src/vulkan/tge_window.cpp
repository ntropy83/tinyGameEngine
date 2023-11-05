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
      vulDebug.writeToBuffer("failed to create window surface", MessageType::Critical);

      throw std::runtime_error("failed to create window surface");
    }
  }

  void TgeWindow::integrateInto(QWidget* placeholder) {
  #ifdef _WIN32
      // Windows-specific code
      HWND win32NativeWindow = glfwGetWin32Window(window);
      HWND win32NativeWindow = reinterpret_cast<HWND>(static_cast<UINT_PTR>(placeholder->winId()));

      // You may need to do additional setup for integrating with Qt on Windows
      // For example, setting parent-child relationships or window styles.

      // Reparenting code specific to Windows, if necessary.
      // Example:
      SetParent(win32NativeWindow, qtNativeWindow);

      // Show the GLFW window
      glfwShowWindow(window);
  #else
      // Linux-specific code (X11)
      Window x11NativeWindow = glfwGetX11Window(window);
      Window qtNativeWindow = static_cast<Window>(placeholder->winId());

      Display* display = glfwGetX11Display();

      // Reparenting code for X11
      XReparentWindow(display, x11NativeWindow, qtNativeWindow, 0, 0);

      // Show the GLFW window
      glfwShowWindow(window);
  #endif
  }

} // namespace tge

