#include "vulkan/tge_model.hpp"


// std
#include <cassert>
#include <cstddef>
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace tge {
        TgeModel::TgeModel(TgeDevice &device, const std::vector<Vertex> &vertices) : tgeDevice{device} {
            createVertexBuffers(vertices);
        }

        TgeModel::~TgeModel() {
            vkDestroyBuffer(tgeDevice.device(), vertexBuffer, nullptr);
            vkFreeMemory(tgeDevice.device(), vertexBufferMemory, nullptr);
        }

        void TgeModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
            vertexCount = static_cast<uint32_t>(vertices.size());
                assert(vertexCount >= 3 && "Vertex count must be at least 3");
            VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
            tgeDevice.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                vertexBuffer,
                vertexBufferMemory);
            
            void *data;
            vkMapMemory(tgeDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
            vkUnmapMemory(tgeDevice.device(), vertexBufferMemory);
        }

        void TgeModel::bind(VkCommandBuffer commandBuffer) {
            VkBuffer buffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        }

        void TgeModel::draw(VkCommandBuffer commandBuffer) {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }

        std::vector<VkVertexInputBindingDescription> TgeModel::Vertex::getBindingDescriptions() {
            return {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}};
        }

        std::vector<VkVertexInputAttributeDescription> TgeModel::Vertex::getAttributeDescriptions() {
            return {{0, 0, VK_FORMAT_R32G32_SFLOAT, 0}};
        }

}