#pragma once

//#include "JsonFiles.hpp"

#include "vulkan/tge_device.hpp"
#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_swap_chain.hpp"
#include "vulkan/tge_window.hpp"
#include "tge_game_object.hpp"

// std
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace tge {

  class TgeEditor {

    public:

      static constexpr int WIDTH = 1280;
      static constexpr int HEIGHT = 800;
      
      TgeEditor();
      ~TgeEditor();

      TgeEditor(const TgeEditor &) = delete;
			TgeEditor &operator=(const TgeEditor &) = delete;

      std::vector<VkCommandBuffer> commandBuffer(){ return commandBuffers; }

      TgeWindow& getWindow() { return tgeWindow; }
      
      void update();

    private:

      void loadGameObjects(); 
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void freeCommandBuffers();
      void drawFrame();
      void recreateSwapChain();
      void recordCommandBuffer(int imageIndex);
      void renderGameObjects(VkCommandBuffer commandBuffer);

      TgeWindow tgeWindow;
      TgeDevice tgeDevice;
      std::unique_ptr<TgeSwapChain> tgeSwapChain;
      std::unique_ptr<TgePipeline> tgePipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::vector<TgeGameObject> gameObjects;

      bool window_should_close(){ return tgeWindow.shouldClose(); }
  };
} // namespace tge