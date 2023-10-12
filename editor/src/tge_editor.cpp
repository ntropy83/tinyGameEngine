#include "tge_editor.hpp"
#include "vulkan/tge_shadercomp.hpp"

//std
#include <array>
#include <stdexcept>

namespace tge {

  TgeEditor::TgeEditor(){
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  TgeEditor::~TgeEditor(){
    vkDestroyPipelineLayout(tgeDevice.device(), pipelineLayout, nullptr);
  }

  void TgeEditor::run() {
    while (!tgeWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();

      /*  if (ui.SwapChainRebuild()){

          ui-> = false;
          ImGui_ImplVulkan_SetMinImageCount(tgeEditorUI.MinImageCount()));
          ImGui_ImplVulkanH_CreateWindow(tgeEditorUI.getInstance(), tgeEditorUI.getPhysicalDevice(), tgeEditorUI.getDevice(), tgeEditorUI.getWindow(), 
          tgeEditorUI.getQueueFamily(), tgeEditorUI.getAllocator(), g_SwapChainResizeWidth, g_SwapChainResizeHeight, tgeEditorUI.MinImageCount());
          g_MainWindowData.FrameIndex = 0;
        } */

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

    /*  memcpy(&wd->ClearValue.color.float32[0], &clear_color, 4 * sizeof(float));
        FrameRender(wd);
        FramePresent(wd); */

      }

      vkDeviceWaitIdle(tgeDevice.device());
  };

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

  void TgeEditor::createPipeline(){
    auto pipelineConfig = 
      TgePipeline::defaultPipelineConfigInfo(tgeSwapChain.width(), tgeSwapChain.height());
    pipelineConfig.renderPass = tgeSwapChain.getRenderPass();
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
    commandBuffers.resize(tgeSwapChain.imageCount());

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

    for(int i = 0; i < commandBuffers.size(); i++){
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = tgeSwapChain.getRenderPass();
    renderPassInfo.framebuffer = tgeSwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = tgeSwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    tgePipeline->bind(commandBuffers[i]);
    vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }
}
void TgeEditor::drawFrame() {
  uint32_t imageIndex;
  auto result = tgeSwapChain.acquireNextImage(&imageIndex);
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  result = tgeSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}
} // namespace tge