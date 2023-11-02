#include "debug/tge_vulDebug.hpp"

namespace tge {

std::ostringstream TgeVulDebug::globalBuffer;
std::mutex TgeVulDebug::bufferMutex;

TgeVulDebug::TgeVulDebug() {}

TgeVulDebug::~TgeVulDebug() {}

void TgeVulDebug::writeToBuffer(const std::string& text, MessageType type) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    std::string coloredText;
    switch (type) {
        case MessageType::Debug:
            coloredText = "<font color='lightgray'>DEBUG: " + text + "</font><br>";
            break;
        case MessageType::Info:
            coloredText = "<font color='lightgray'>INFO: " + text + "</font><br>";
            break;
        case MessageType::Warning:
            coloredText = "<font color='orange'>WARNING: " + text + "</font><br>";
            break;
        case MessageType::Critical:
            coloredText = "<font color='red'>CRITICAL: " + text + "</font><br>";
            break;
        case MessageType::Fatal:
            coloredText = "<font color='red'>FATAL: " + text + "</font><br>";
            break;
    }
    globalBuffer << coloredText;
}

std::string TgeVulDebug::flushBuffer() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    std::string content = globalBuffer.str();

    globalBuffer.str("");  // Clear the buffer
    globalBuffer.clear();  // Clear any error flags

    // If the content is not empty, wrap it in HTML body tags
    if (!content.empty()) {
        content = "<html><body>" + content + "</body></html>";
    }

    return content;
}

} // namespace tge
