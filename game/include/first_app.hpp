#pragma once

#include "file/filesystem.hpp"
#include "JsonFiles.hpp"

#include "vulkan/tge_window.hpp"
#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_device.hpp"
#include "vulkan/tge_shadercomp.hpp"

namespace tge {

  // const GameConfig gameConfig(FileSystem::Read("game.json"));

  class FirstApp {

    public:

      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;
      
      void run();
    
    private:

      TgeWindow tgeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      TgeDevice tgeDevice{tgeWindow};
      TgeShaderCompiler compiler{"shaders_includes/"};
      TgePipeline tgePipeline{
        tgeDevice,
        compiler, 
        "shaders/simple_shader.vert", 
        "shaders/simple_shader.frag",
        TgePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
  };
} // namespace tge