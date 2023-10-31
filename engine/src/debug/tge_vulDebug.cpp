#include "debug/tge_vulDebug.hpp"

namespace tge {

std::ostringstream TgeVulDebug::globalBuffer;
std::mutex TgeVulDebug::bufferMutex;

TgeVulDebug::TgeVulDebug() {}

TgeVulDebug::~TgeVulDebug() {}

void TgeVulDebug::writeToBuffer(const std::string& text) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    globalBuffer << text;
}

std::string TgeVulDebug::flushBuffer() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    std::string content = globalBuffer.str();
    
    globalBuffer.str("");  // Clear the buffer
    globalBuffer.clear();  // Clear any error flags
    return content;
}

} // namespace tge