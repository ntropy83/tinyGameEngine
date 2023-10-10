#pragma once

#include "file/filesystem.hpp"

#include "vulkan/tge_window.hpp"
#include "vulkan/tge_pipeline.hpp"

namespace tge {

  class FirstApp {

    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      void run();
    
    private:
      TgeWindow tgeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      TgePipeline tgePipeline{"shaders/simple_shader.vert", "shaders/simple_shader.frag"};
  };
} // namespace tge