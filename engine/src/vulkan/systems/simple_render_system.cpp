#include "vulkan/systems/simple_render_system.hpp"
#include "vulkan/tge_shadercomp.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_TO_ZERO
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

//std
#include <array>
#include <cassert>
#include <stdexcept>

namespace tge {

  struct SimplePushConstantData {
    glm::mat4 transform{1.f};
    alignas(16) glm::vec3 color;
  };

  SimpleRenderSystem::SimpleRenderSystem(TgeDevice &device, VkRenderPass renderPass) : tgeDevice{device}
  {
    createPipelineLayout();
    createPipeline(renderPass);
  }

  SimpleRenderSystem::~SimpleRenderSystem(){
    vkDestroyPipelineLayout(tgeDevice.device(), pipelineLayout, nullptr);
  }

  void SimpleRenderSystem::createPipelineLayout(){

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if(vkCreatePipelineLayout(
        tgeDevice.device(), 
        &pipelineLayoutInfo, 
        nullptr, &pipelineLayout) != VK_SUCCESS){
      throw std::runtime_error("Failed to create pipeline layout ...");
    }
  }

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  TgePipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;

    TgeShaderCompiler compiler{"shaders_includes/"};

    tgePipeline = std::make_unique<TgePipeline>(
        tgeDevice,
        compiler, 
        "shaders/simple_shader.vert", 
        "shaders/simple_shader.frag",
        pipelineConfig);
  }

  void SimpleRenderSystem::renderGameObjects(
          VkCommandBuffer commandBuffer, 
          std::vector<TgeGameObject> &gameObjects,
          const TgeCamera &camera) {
    tgePipeline->bind(commandBuffer);

    for (auto& obj : gameObjects) {
      obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
      obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

      for(int j = 0; j < 4; j++) {
        SimplePushConstantData push{};
        push.color = obj.color;
        push.transform = camera.getProjection() * obj.transform.mat4();

        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                            0, sizeof(SimplePushConstantData), &push);
      }
      obj.model->bind(commandBuffer);
      obj.model->draw(commandBuffer);
    }
  }
} // namespace tge