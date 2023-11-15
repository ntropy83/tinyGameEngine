#pragma once

//#include "JsonFiles.hpp"

#include "vulkan/tge_device.hpp"
#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_swap_chain.hpp"
#include "vulkan/tge_window.hpp"
#include "vulkan/tge_model.hpp"

// ImGUI
//#include "ui/tge_gui.hpp"

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

      TgeWindow& getWindow() { return tgeWindow; }
      
      void update();

    private:

      void loadModels(); 
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void drawFrame();

      TgeWindow tgeWindow;
      TgeDevice tgeDevice;
      TgeSwapChain tgeSwapChain;
      std::unique_ptr<TgePipeline> tgePipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::unique_ptr<TgeModel> tgeModel;

      bool window_should_close(){ return tgeWindow.shouldClose(); }
  };
} // namespace tge