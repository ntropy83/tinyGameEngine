#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace tge {
    class TgeVulDebug {
        public:

        	TgeVulDebug();
			~TgeVulDebug();

			TgeVulDebug(const TgeVulDebug &) = delete;
			TgeVulDebug &operator=(const TgeVulDebug &) = delete;

            std::ostringstream globalBuffer;
            void writeToBuffer(const std::string& text);
            std::string flushBuffer();

    };

}