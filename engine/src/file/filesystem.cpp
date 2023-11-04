#include "file/filesystem.hpp"
#include "physfs/src/physfs.h"

//std 
#include <fstream>
#include <stdexcept>
#include <iostream>

  std::vector<char> FileSystem::read_binary(const std::string& filepath) {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    if(!file.is_open()) {
      throw std::runtime_error("failed to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(8);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;    
  }

  static const char *GetLastError() {
      return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
  }

  void FileSystem::Init() {
      if(!PHYSFS_init(nullptr)){ std::cout << "Failed to init PhysFS: " << GetLastError() << "\n"; }
      std::cout << "PhysFS " << PHYSFS_VER_MAJOR << PHYSFS_VER_MINOR << PHYSFS_VER_PATCH << "\n";
  }

  void FileSystem::Shutdown() {
      if(!PHYSFS_deinit()){ std::cout << "Failed to init PhysFS: " << GetLastError() << "\n"; }
  }

  void FileSystem::Mount(const std::string &newDir, const std::string &mountPoint, bool appendToPath) {
      if (!PHYSFS_mount(newDir.c_str(), mountPoint.c_str(), appendToPath)) {
          std::cout << "PhysFS failed to mount " << newDir.c_str() << " at " << mountPoint.c_str() << ": " << GetLastError() << "\n";
      } else {
          std::cout << "PhysFS mount " << newDir.c_str() << " at " << mountPoint.c_str() << "\n";
      }
  }

  std::string FileSystem::Read(const std::string &filename) {
      PHYSFS_File *file = PHYSFS_openRead(filename.c_str());
      if (!file) {
          std::cout << "PhysFS failed to open file " << filename.c_str() << ": " << GetLastError() << "\n";
          return {};
      }
      const PHYSFS_sint64 length = PHYSFS_fileLength(file);
      if (length == -1) {
          std::cout << "PhysFS failed to get file size " << filename.c_str() << ": " << GetLastError() << "\n";
          return {};
      }
      std::string bytes(length, '\0');
      if (PHYSFS_readBytes(file, const_cast<void*>(static_cast<const void*>(bytes.data())), length) == -1) {
          std::cout << "PhysFS failed to read file " << filename.c_str() << ": {}" << GetLastError() << "\n";
      }
      return bytes;
  }