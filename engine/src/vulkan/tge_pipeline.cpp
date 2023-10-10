#include "vulkan/tge_pipeline.hpp"
#include "vulkan/tge_shadercomp.hpp"

// std
#include <iostream>

namespace tge {

  TgePipeline::TgePipeline(const std::string& vertFilepath, const std::string& fragFilepath){
   createGraphicsPipeline(vertFilepath, fragFilepath);
  }

  void TgePipeline::createGraphicsPipeline(
    const std::string& vertFilepath, const std::string& fragFilepath) {

      auto vertCode = vertFilepath;
      auto fragCode = fragFilepath;

      std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
      std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
  }
}