#pragma once

#include <QWidget>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

			void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

		private:

			void initWindow();

			const int width;
			const int height;

			std::string windowName;
			GLFWwindow *window;
	};
} // namespace tge