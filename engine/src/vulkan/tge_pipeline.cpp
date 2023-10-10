#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_shadercomp.hpp"
#include "vulkan/tge_pipeline_config.hpp"

// std
#include <iostream>

namespace tge {

   void TgePipeline::createGraphicsPipeline(
    const std::string& vertFilepath, const std::string& fragFilepath) {

      auto vertCode = FileSystem::read_binary(vertFilepath);
      auto fragCode = FileSystem::read_binary(fragFilepath);

      std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
      std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }
}