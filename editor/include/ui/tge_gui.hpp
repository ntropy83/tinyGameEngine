#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

namespace tge {
  class TgeEditorUI {
    public:

      TgeEditorUI();
      ~TgeEditorUI();

      TgeEditorUI(const TgeEditorUI &) = delete;
			TgeEditorUI &operator=(const TgeEditorUI &) = delete;

      void imgui_init();

    private:
      GLFWwindow          *window;
      VkInstance&         g_Instance;
      VkPhysicalDevice&   g_PhysicalDevice;
      VkDevice&           g_Device;
      VkQueueFamily       g_QueueFamily;
      VkQueue             g_Queue;
      VkPipelineCache     g_PipelineCache;
      VkDescriptorPool    g_DescriptorPool;
      Allocator           g_Allocator;
  };
}