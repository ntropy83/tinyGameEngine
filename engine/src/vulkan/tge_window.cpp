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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

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

  void TgeWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto tgeWindow = reinterpret_cast<TgeWindow *>(glfwGetWindowUserPointer(window));
    tgeWindow->framebufferResized = true;
    tgeWindow->width = width;
    tgeWindow->height = height;
  }

  void TgeWindow::integrateInto(QWidget* placeholder) {
    #ifdef _WIN32
      // Windows-specific code
      HWND win32NativeWindow = glfwGetWin32Window(window); // Get the GLFW window's native handle

      HWND qtNativeWindow = reinterpret_cast<HWND>(static_cast<UINT_PTR>(placeholder->winId())); // Get the Qt placeholder's native handle

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

    void TgeWindow::dockInto(QWidget* placeholderWidget) {
        #ifdef _WIN32
            HWND winHandle = glfwGetWin32Window(window);
            HWND qtWinHandle = (HWND)placeholderWidget->winId();
            // Windows-specific docking logic
            // ...
        #elif defined(__linux__)
            Window x11NativeWindow = glfwGetX11Window(window);
            Window qtNativeWindow = (Window)placeholderWidget->winId();
            Display* display = glfwGetX11Display();
            XReparentWindow(display, x11NativeWindow, qtNativeWindow, 0, 0);
            XResizeWindow(display, x11NativeWindow, placeholderWidget->width(), placeholderWidget->height());
            XMapWindow(display, x11NativeWindow);
        #endif
    }

    void TgeWindow::undockFrom(QWidget* placeholderWidget) {
        #ifdef _WIN32
            HWND winHandle = glfwGetWin32Window(window);
            // Windows-specific undocking logic
            // ...
        #elif defined(__linux__)
            Window x11NativeWindow = glfwGetX11Window(window);
            Display* display = glfwGetX11Display();
            XReparentWindow(display, x11NativeWindow, DefaultRootWindow(display), 200, 200);
            XMapWindow(display, x11NativeWindow);
        #endif
    }

} // namespace tge

