#pragma once

#include "vulkan/tge_device.hpp"
#include <vulkan/vulkan_core.h>

// GLM Lib defines
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

// std
#include <vector>

namespace tge {
    class TgeModel {
        public:

        struct Vertex {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        TgeModel(TgeDevice &device, const std::vector<Vertex> &vertices);
        ~TgeModel();

        TgeModel(const TgeModel &) = delete;
		    	TgeModel &operator=(const TgeModel &) = delete;
        
        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);

            TgeDevice &tgeDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;
    };
}