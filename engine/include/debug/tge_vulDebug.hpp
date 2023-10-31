#ifndef TGE_VULDEBUG_HPP
#define TGE_VULDEBUG_HPP

#include <sstream>
#include <mutex>

namespace tge {

class TgeVulDebug {
public:
    TgeVulDebug();
    ~TgeVulDebug();

    void writeToBuffer(const std::string& text);
    std::string flushBuffer();

private:
    static std::ostringstream globalBuffer;
    static std::mutex bufferMutex;
};

} // namespace tge

#endif // TGE_VULDEBUG_HPP
