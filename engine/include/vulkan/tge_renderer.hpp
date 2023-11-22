#pragma once

#include "vulkan/tge_device.hpp"
#include "vulkan/tge_swap_chain.hpp"
#include "vulkan/tge_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace tge {

  class TgeRenderer {

    public:

     TgeRenderer(TgeWindow &window, TgeDevice &device);
     ~TgeRenderer();

     TgeRenderer(const TgeRenderer &) = delete;
	 TgeRenderer &operator=(const TgeRenderer &) = delete;

     VkRenderPass getSwapChainRenderPass() const { return tgeSwapChain->getRenderPass(); }
     bool isFrameInProgress() const { return isFrameStarted; }

     VkCommandBuffer getCurrentCommandBuffer() {
        assert(isFrameStarted && "Cannot get command buffer, when frame not in progress.");
        return commandBuffers[currentFrameIndex];
     }

     int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index, when frame not in progress.");
        return currentFrameIndex;
     }

     VkCommandBuffer beginFrame();
     void endFrame();
     void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
     void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:

      void createCommandBuffers();
      void freeCommandBuffers();
      void recreateSwapChain();

      TgeWindow& tgeWindow;
      TgeDevice& tgeDevice;
      std::unique_ptr<TgeSwapChain> tgeSwapChain;
      std::vector<VkCommandBuffer> commandBuffers;

      uint32_t currentImageIndex;
      int currentFrameIndex{0};
      bool isFrameStarted = false;
  };
} // namespace tge