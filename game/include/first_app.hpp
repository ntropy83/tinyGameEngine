#pragma once

#include "file/filesystem.hpp"
#include "JsonFiles.hpp"

#include "vulkan/tge_window.hpp"
#include "vulkan/tge_pipeline.hpp"

namespace tge {

  // const GameConfig gameConfig(FileSystem::Read("game.json"));

  class FirstApp {

    public:

      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      void run();
    
    private:
      TgeWindow tgeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      TgePipeline tgePipeline{FileSystem::Read("shaders/simple_shader.vert.spv"), FileSystem::Read("shaders/simple_shader.frag.spv")};
  };
} // namespace tge