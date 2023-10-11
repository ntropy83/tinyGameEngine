#include "first_app.hpp"
#include "vulkan/tge_shadercomp.hpp"

//std
#include <stdexcept>

namespace tge {

  FirstApp::FirstApp(){
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  FirstApp::~FirstApp(){
    vkDestroyPipelineLayout(tgeDevice.device(), pipelineLayout, nullptr);
  }

  void FirstApp::run() {
    while (!tgeWindow.shouldClose()) {
        glfwPollEvents();
      }
  };

  void FirstApp::createPipelineLayout(){
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

  void FirstApp::createPipeline(){
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

  void FirstApp::createCommandBuffers(){
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

  }

  void FirstApp::drawFrame(){

  }

} // namespace tge