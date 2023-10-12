#pragma once

// std
#include <string>

struct GameConfig {
    std::string Name;
    int         Width;
    int         Height;
    bool        Maximized;
    std::string StartMap;

    explicit GameConfig(const std::string &jsonFilename);
};