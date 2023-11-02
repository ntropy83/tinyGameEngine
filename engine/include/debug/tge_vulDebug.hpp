#ifndef TGE_VULDEBUG_HPP
#define TGE_VULDEBUG_HPP

#include <sstream>
#include <mutex>

namespace tge {

enum class MessageType {
    Debug,
    Info,
    Warning,
    Critical,
    Fatal
};

class TgeVulDebug {
public:
    TgeVulDebug();
    ~TgeVulDebug();

    void writeToBuffer(const std::string& text, MessageType type);
    std::string flushBuffer();

private:
    static std::ostringstream globalBuffer;
    static std::mutex bufferMutex;
};

} // namespace tge

#endif // TGE_VULDEBUG_HPP
