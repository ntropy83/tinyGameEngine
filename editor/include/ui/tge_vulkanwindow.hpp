#pragma once

//#include "JsonFiles.hpp"

#include "vulkan/tge_device.hpp"
#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_swap_chain.hpp"
#include "vulkan/tge_window.hpp"
#include "vulkan/tge_model.hpp"

// std
#include <memory>
#include <vector>

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

      void loadModels(); 
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void freeCommandBuffers();
      void drawFrame();
      void recreateSwapChain();
      void recordCommandBuffer(int imageIndex);

      TgeWindow tgeWindow;
      TgeDevice tgeDevice;
      std::unique_ptr<TgeSwapChain> tgeSwapChain;
      std::unique_ptr<TgePipeline> tgePipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::unique_ptr<TgeModel> tgeModel;

      bool window_should_close(){ return tgeWindow.shouldClose(); }
  };
} // namespace tge