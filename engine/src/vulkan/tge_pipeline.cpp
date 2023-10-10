#include "vulkan/tge_pipeline.hpp"

// std
#include <iostream>

namespace tge {

   void TgePipeline::createGraphicsPipeline(
    const std::string& vertFilepath, const std::string& fragFilepath) {

      auto vertCode = vertFilepath;
      auto fragCode = vertFilepath;

      std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
      std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }
}