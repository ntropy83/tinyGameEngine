#pragma once

//#include "JsonFiles.hpp"

#include "vulkan/tge_device.hpp"
#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_swap_chain.hpp"
#include "vulkan/tge_window.hpp"

// ImGUI
#include "ui/tge_gui.hpp"

// std
#include <memory>
#include <vector>

namespace tge {

  // const GameConfig gameConfig(FileSystem::Read("game.json"));

  class TgeEditor {

    public:

      static constexpr int WIDTH = 1280;
      static constexpr int HEIGHT = 800;
      
      TgeEditor();
      ~TgeEditor();

      TgeEditor(const TgeEditor &) = delete;
			TgeEditor &operator=(const TgeEditor &) = delete;

      std::vector<VkCommandBuffer> commandBuffer(){ return commandBuffers; }
      
      void run();
    
    private:
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void drawFrame();

      TgeWindow tgeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      TgeDevice tgeDevice{tgeWindow};
      TgeSwapChain tgeSwapChain{tgeDevice, tgeWindow.getExtent()};
      std::unique_ptr<TgePipeline> tgePipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
  };
} // namespace tge