#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include "vulkan/tge_swap_chain.hpp"

// std
#include <memory>

namespace tge {
  class TgeEditorUI {
    public:

      TgeEditorUI();
      ~TgeEditorUI();

      TgeEditorUI(const TgeEditorUI &) = delete;
			TgeEditorUI &operator=(const TgeEditorUI &) = delete;

      void imgui_init();
      static void check_vk_result(VkResult err);

      GLFWwindow* getWindow(){ return window; }
      VkInstance getInstance(){ return g_Instance; }
      VkPhysicalDevice getPhysicalDevice(){ return g_PhysicalDevice; }
      VkDevice getDevice(){ return g_Device; }
      uint32_t getQueueFamily(){ return g_QueueFamily; }
      VkAllocationCallbacks* getAllocator(){ return g_Allocator; }
      int MinImageCount(){ return g_MinImageCount; }
      bool SwapChainRebuild(){ return g_SwapChainRebuild; }

    private:
      GLFWwindow*                    window;
      VkInstance                     g_Instance;
      VkPhysicalDevice               g_PhysicalDevice;
      VkSurfaceKHR                   g_Surface;
      VkDevice                       g_Device;
      uint32_t                       g_QueueFamily = (uint32_t)-1;;
      VkQueue                        g_Queue;
      VkPipelineCache                g_PipelineCache;
      VkDescriptorPool               g_DescriptorPool;
      VkAllocationCallbacks*         g_Allocator;
      int                            g_MinImageCount = 2;
      bool                           g_SwapChainRebuild = false;

      TgeSwapChain                   swapChain;
      VkRenderPass                   imGuiRenderPass;
      VkCommandBuffer                commandBuffers;
  };
}