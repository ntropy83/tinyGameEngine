#pragma once

#include "file/filesystem.hpp"

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace tge {

  class TgePipeline {
    public:
      TgePipeline(
        const std::string&      vertFilepath, 
        const std::string&      fragFilepath);

    private:
      void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
  };
} // namespace tge