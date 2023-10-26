#include "debug/tge_vulDebug.hpp"
#include <sstream>
#include <string>

namespace tge {

    TgeVulDebug::TgeVulDebug(){}

    TgeVulDebug::~TgeVulDebug(){}

    void TgeVulDebug::writeToBuffer(const std::string& text) {
        globalBuffer << text;
    }

    std::string TgeVulDebug::flushBuffer() {
        return globalBuffer.str();
        globalBuffer.str("");  // Clear the buffer
        globalBuffer.clear();  // Clear any error flags
    }
}