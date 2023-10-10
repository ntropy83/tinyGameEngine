#pragma once

// std
#include <vector>
#include <string>

namespace FileSystem {
  
    std::vector<char> read_binary(const std::string& filepath);

    void Init();

    void Shutdown();

    void Mount(const std::string &newDir, const std::string &mountPoint, bool appendToPath = true);

    std::string Read(const std::string &filename);

} // namespace tge