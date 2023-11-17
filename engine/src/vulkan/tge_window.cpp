#include "vulkan/tge_window.hpp"
#include "debug/tge_vulDebug.hpp"
#include "GLFW/glfw3.h"

// include std
#include <stdexcept>
#include <iostream>

namespace tge {

    TgeWindow::TgeWindow(int w, int h, std::string name, QWidget *parent) 
        : width{w}, height{h}, windowName{name} {
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
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
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

    void TgeWindow::integrateInto(QTabWidget* vulkanWindowWidget, int tabIndex) {
        QWidget* tab = vulkanWindowWidget->widget(tabIndex);
        auto nativeWindow = getNativeWindowHandle();

        #ifdef _WIN32
            SetParent(nativeWindow, (HWND)tab->winId());
            SetWindowPos(nativeWindow, HWND_TOP, 0, 0, tab->width(), tab->height(), SWP_SHOWWINDOW);
        #else
            Display* display = glfwGetX11Display();
            XReparentWindow(display, nativeWindow, tab->winId(), 0, 0);
            XResizeWindow(display, nativeWindow, tab->width(), tab->height());
            XMapWindow(display, nativeWindow);
        #endif
    }

    void TgeWindow::undockFrom(QWidget* placeholderWidget) {
        auto nativeWindow = getNativeWindowHandle();

        #ifdef _WIN32
            SetParent(nativeWindow, NULL); // Detach from parent
            SetWindowLongPtr(nativeWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
            SetWindowPos(nativeWindow, HWND_TOP, 200, 200, 800, 600, SWP_SHOWWINDOW);
            SetForegroundWindow(nativeWindow);
            SetFocus(nativeWindow);
        #else
            Display* display = glfwGetX11Display();
            XReparentWindow(display, nativeWindow, DefaultRootWindow(display), 200, 200);
            XMapRaised(display, nativeWindow);
            XSetInputFocus(display, nativeWindow, RevertToParent, CurrentTime);
        #endif
    }
} // namespace tge
