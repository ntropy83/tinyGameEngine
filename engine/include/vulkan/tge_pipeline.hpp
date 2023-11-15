#pragma once

#include "file/filesystem.hpp"
#include "vulkan/tge_device.hpp"

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace tge {

  class TgeShaderCompiler;

  struct PipelineConfigInfo{
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
      TgePipeline &operator = (const TgePipeline&) = delete;

      void bind(VkCommandBuffer commandBuffer);

      static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

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