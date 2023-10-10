#include "vulkan/tge_pipeline_config.hpp"

#include "file/JsonFile.hpp"

namespace tge {
    VulkanPipelineConfig::VulkanPipelineConfig(const std::string &jsonFilename) {
        JsonFile pipelineJson(jsonFilename);
        VertexShader                  = pipelineJson.GetString("vertex");
        FragmentShader                = pipelineJson.GetString("fragment");
    }
}