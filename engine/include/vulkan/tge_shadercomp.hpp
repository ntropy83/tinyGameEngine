#pragma once

#include "file/filesystem.hpp"

// std
#include <vector>
#include <string>
#include <memory>

#include <vulkan/vulkan.hpp>

namespace tge {

#include <vulkan/vulkan.hpp>

struct ShaderIncluder;

class TgeShaderCompiler {
public:
    explicit TgeShaderCompiler(const std::string &includesDir);

    ~TgeShaderCompiler();

    TgeShaderCompiler(const TgeShaderCompiler&) = delete;
    TgeShaderCompiler &operator=(const TgeShaderCompiler&) = delete;

    std::vector<uint32_t> Compile(vk::ShaderStageFlagBits stage, const std::string &source);

    std::vector<uint32_t> CompileFromFile(vk::ShaderStageFlagBits stage, const std::string &filename);

private:
    std::unique_ptr<ShaderIncluder> m_includer;
};
} // namespace tge