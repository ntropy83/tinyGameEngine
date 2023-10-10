#include "JsonFiles.hpp"

#include "file/JsonFile.hpp"

GameConfig::GameConfig(const std::string &jsonFilename) {
    JsonFile json(jsonFilename);
    Name      = json.GetString("name");
    Width     = json.GetInteger("width");
    Height    = json.GetInteger("height");
    Maximized = json.GetBoolean("maximized");
    StartMap  = json.GetString("start_map");
}