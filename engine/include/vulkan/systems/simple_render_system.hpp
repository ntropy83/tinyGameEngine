#pragma once

//#include "JsonFiles.hpp"

#include "vulkan/tge_device.hpp"
#include "vulkan/tge_pipeline.hpp"
#include "tge_game_object.hpp"

// std
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace tge {

  class SimpleRenderSystem {

    public:
      
      SimpleRenderSystem( TgeDevice &device, VkRenderPass renderPass);
      ~SimpleRenderSystem();

      SimpleRenderSystem(const SimpleRenderSystem &) = delete;
			SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;
      
      void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<TgeGameObject> &gameObjects);

    private:

      void createPipelineLayout();
      void createPipeline(VkRenderPass renderPass);

      TgeDevice& tgeDevice;

      std::unique_ptr<TgePipeline> tgePipeline;
      VkPipelineLayout pipelineLayout;
  };
} // namespace tge