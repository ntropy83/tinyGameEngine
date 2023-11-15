#include "ui/tge_vulkanwindow.hpp"
#include "vulkan/tge_shadercomp.hpp"
#include "vulkan/tge_window.hpp"

//std
#include <array>
#include <cassert>
#include <stdexcept>

namespace tge {

  TgeEditor::TgeEditor() :
      tgeWindow(WIDTH, HEIGHT, "Tge_VulkanWindow"),
      tgeDevice(tgeWindow),
      tgeSwapChain()
  {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
  }

  TgeEditor::~TgeEditor(){
    vkDestroyPipelineLayout(tgeDevice.device(), pipelineLayout, nullptr);
  }

  void TgeEditor::update() {
        glfwPollEvents();
        drawFrame();
        vkDeviceWaitIdle(tgeDevice.device());
  };

  void TgeEditor::loadModels() {
    std::vector<TgeModel::Vertex> vertices{
        {{0.0f, -0.9f}, {1.0f, 0.0f, 0.0f}},
        {{0.9f, 0.9f}, {0.0f, 1.0f, 0.0f}},
        {{-0.9f, 0.9f}, {0.0f, 0.0f, 1.0f}}};
    
    tgeModel = std::make_unique<TgeModel>(tgeDevice, vertices);
  }

  void TgeEditor::createPipelineLayout(){
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if(vkCreatePipelineLayout(
        tgeDevice.device(), 
        &pipelineLayoutInfo, 
        nullptr, &pipelineLayout) != VK_SUCCESS){
      throw std::runtime_error("Failed to create pipeline layout ...");
    }
  }

  void TgeEditor::recreateSwapChain() {
    auto extent = tgeWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
      extent = tgeWindow.getExtent();
      glfwWaitEvents();
    }
    vkDeviceWaitIdle(tgeDevice.device());

    if (tgeSwapChain == nullptr) {
    tgeSwapChain = std::make_unique<TgeSwapChain>(tgeDevice, extent);
    } else {
      tgeSwapChain = std::make_unique<TgeSwapChain>(tgeDevice, extent, std::move(tgeSwapChain));
      if (tgeSwapChain->imageCount() != commandBuffers.size()) {
        freeCommandBuffers();
        createCommandBuffers();
      }
    }
    createPipeline();
}

void TgeEditor::createPipeline() {
  assert(tgeSwapChain != nullptr && "Cannot create pipeline before swap chain");
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  TgePipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = tgeSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;

    TgeShaderCompiler compiler{"shaders_includes/"};

    tgePipeline = std::make_unique<TgePipeline>(
        tgeDevice,
        compiler, 
        "shaders/simple_shader.vert", 
        "shaders/simple_shader.frag",
        pipelineConfig);
  }

  void TgeEditor::createCommandBuffers(){
    commandBuffers.resize(tgeSwapChain->imageCount());
    
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

  void TgeEditor::freeCommandBuffers() {
    vkFreeCommandBuffers(
    tgeDevice.device(),
    tgeDevice.getCommandPool(),
    static_cast<uint32_t>(commandBuffers.size()),
    commandBuffers.data());
    commandBuffers.clear();
  }

  void TgeEditor::recordCommandBuffer(int imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

  VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = tgeSwapChain->getRenderPass();
    renderPassInfo.framebuffer = tgeSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = tgeSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(tgeSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(tgeSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, tgeSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    tgePipeline->bind(commandBuffers[imageIndex]);
    tgeModel->bind(commandBuffers[imageIndex]);
    tgeModel->draw(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void TgeEditor::drawFrame() {
    uint32_t imageIndex;
    auto result = tgeSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = tgeSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      tgeWindow.wasWindowResized()) {
      tgeWindow.resetWindowResizedFlag();
      recreateSwapChain();
      return;
    } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }
  }
} // namespace tge