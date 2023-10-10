#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace tge {

	class TgeWindow {
		public:
			TgeWindow(int w, int h, std::string name);
			~TgeWindow();

			TgeWindow(const TgeWindow &) = delete;
			TgeWindow &operator=(const TgeWindow &) = delete;

			bool shouldClose() { return glfwWindowShouldClose(window); }

			void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

		private:

			void initWindow();

			const int width;
			const int height;

			std::string windowName;
			GLFWwindow *window;
	};
} // namespace tge