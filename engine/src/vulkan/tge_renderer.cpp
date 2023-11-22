#include "vulkan/tge_renderer.hpp"
#include "vulkan/tge_device.hpp"
#include "vulkan/tge_swap_chain.hpp"
#include "vulkan/tge_window.hpp"

//std
#include <array>
#include <cassert>
#include <stdexcept>

namespace tge {

  TgeRenderer::TgeRenderer(TgeWindow &window, TgeDevice &device) : tgeWindow{window}, tgeDevice{device}
  {
    recreateSwapChain();
    createCommandBuffers();
  }

  TgeRenderer::~TgeRenderer(){
    freeCommandBuffers();
  }

  void TgeRenderer::recreateSwapChain() {
    auto extent = tgeWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
      extent = tgeWindow.getExtent();
      glfwWaitEvents();
    }
    vkDeviceWaitIdle(tgeDevice.device());

    if (tgeSwapChain == nullptr) {
    tgeSwapChain = std::make_unique<TgeSwapChain>(tgeDevice, extent);
    } else {
      std::shared_ptr<TgeSwapChain> oldSwapChain = std::move(tgeSwapChain);
      tgeSwapChain = std::make_unique<TgeSwapChain>(tgeDevice, extent, oldSwapChain);

      if (!oldSwapChain->compareSwapFormats(*tgeSwapChain.get())) {
        throw std::runtime_error("SwapChain Image format (or depth) has changed !");
      }
    }  
  }

  void TgeRenderer::createCommandBuffers(){
    commandBuffers.resize(TgeSwapChain::MAX_FRAMES_IN_FLIGHT);
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = tgeDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(
        tgeDevice.device(), 
        &allocInfo, 
        commandBuffers.data()) != VK_SUCCESS){
      throw std::runtime_error("Failed to allocate command buffers ...");
    }
  }

  void TgeRenderer::freeCommandBuffers() {
    vkFreeCommandBuffers(
    tgeDevice.device(),
    tgeDevice.getCommandPool(),
    static_cast<uint32_t>(commandBuffers.size()),
    commandBuffers.data());
    commandBuffers.clear();
  }

  VkCommandBuffer TgeRenderer::beginFrame() {
    assert(!isFrameStarted && "Cant call begin frame, while already in progress.");

    auto result = tgeSwapChain->acquireNextImage(&currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    return commandBuffer;
  }
  void TgeRenderer::endFrame() {
    assert(isFrameStarted && "Cannot call endFrame while frame is not in progress.");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }    

    auto result = tgeSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);  
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      tgeWindow.wasWindowResized()) {
      tgeWindow.resetWindowResizedFlag();
      recreateSwapChain();
    } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }

    isFrameStarted = false; 
    currentFrameIndex = (currentFrameIndex + 1) & TgeSwapChain::MAX_FRAMES_IN_FLIGHT;   
  }

  void TgeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Cannot call beginSwapChainRenderPass if frame is not in progress.");
    assert(commandBuffer == getCurrentCommandBuffer() 
                && "Cannot begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = tgeSwapChain->getRenderPass();
    renderPassInfo.framebuffer = tgeSwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = tgeSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(tgeSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(tgeSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, tgeSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);    
  }

  void TgeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Cannot call endSwapChainRenderPass if frame is not in progress.");
    assert(commandBuffer == getCurrentCommandBuffer() 
                && "Cannot end render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);
  }


} // namespace tge