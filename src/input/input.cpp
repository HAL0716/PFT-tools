#include "input.hpp"

bool loadConfig(const std::string& configPath, nlohmann::json& config) {
    std::ifstream configFile(configPath);
    if (!configFile) {
        std::cerr << "Error: Could not open config file: " << configPath << std::endl;
        return false;
    }
    try {
        configFile >> config;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse config file: " << e.what() << std::endl;
        return false;
    }
    return true;
}
