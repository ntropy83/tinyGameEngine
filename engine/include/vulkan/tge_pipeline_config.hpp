#pragma once

#include <string>

namespace tge {
    struct VulkanPipelineConfig {
        std::string           VertexShader;
        std::string           FragmentShader;
    
        explicit VulkanPipelineConfig(const std::string &jsonFilename);
    };
}