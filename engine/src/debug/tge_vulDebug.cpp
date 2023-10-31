#include "debug/tge_vulDebug.hpp"
#include <sstream>
#include <string>
#include <mutex>

namespace tge {

    std::mutex bufferMutex;

    TgeVulDebug::TgeVulDebug(){}

    TgeVulDebug::~TgeVulDebug(){}

    void TgeVulDebug::writeToBuffer(const std::string& text) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        globalBuffer << text;
    }

    std::string TgeVulDebug::flushBuffer() {
        std::lock_guard<std::mutex> lock(bufferMutex);
        std::string data = globalBuffer.str();
        globalBuffer.str("");
        globalBuffer.clear();
        return data;
    }
}
