#pragma once

#include "file/filesystem.hpp"

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace tge {

  class TgeShaderCompiler;

  class TgePipeline {
    public:
      TgePipeline(
        const std::string&      vertFilepath, 
        const std::string&      fragFilepath);

    private:
      const std::string test;
      void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
  };
} // namespace tge