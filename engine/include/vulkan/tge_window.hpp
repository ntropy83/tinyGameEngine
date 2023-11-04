#pragma once

#include <QWidget>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#ifdef __linux__
	#define GLFW_EXPOSE_NATIVE_X11
	#include <GLFW/glfw3native.h>
	#include <X11/Xlib.h>
	#undef None
#endif

#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
	#include <Windows.h>
#endif

#include <string>

namespace tge {

	class TgeWindow : public QWidget {

		Q_OBJECT

		public:
			TgeWindow(int w, int h, std::string name, QWidget *parent = nullptr);
			~TgeWindow();

			TgeWindow(const TgeWindow &) = delete;
			TgeWindow &operator=(const TgeWindow &) = delete;

			bool shouldClose() { return glfwWindowShouldClose(window); }
			VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
			int get_window_width() { return width; }
			int get_window_height() { return height; }

    		GLFWwindow* getGLFWWindow() {
        		return window;
    		}

			void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

			void integrateInto(QWidget* placeholder);

		private:

			GLFWwindow *window;
			void initWindow();

			const int width;
			const int height;

			std::string windowName;
	};
} // namespace tge