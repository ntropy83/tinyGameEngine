#pragma once

#include "file/filesystem.hpp"
#include "vulkan/tge_device.hpp"

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace tge {

  class TgeShaderCompiler;

  struct PipelineConfigInfo{
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
  };

  class TgePipeline {
    public:
      TgePipeline(
        TgeDevice&                    device,
        TgeShaderCompiler&            compiler,
        const std::string&            vertFilepath, 
        const std::string&            fragFilepath,
        const PipelineConfigInfo&     configInfo);
      ~TgePipeline();

      TgePipeline(const TgePipeline&) = delete;
      void operator = (const TgePipeline&) = delete;

      void bind(VkCommandBuffer commandBuffer);

      static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

      void CreateShaderModule(const std::vector<uint32_t>& spirv, VkShaderModule& shaderModule);

    private:
      void createGraphicsPipeline(
        const PipelineConfigInfo&     configInfo);

      void createShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);
      TgeDevice&            tgeDevice;
      VkPipeline            graphicsPipeline;
      VkShaderModule        vertShaderModule;
      VkShaderModule        fragShaderModule;
  };
} // namespace tge