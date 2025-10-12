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

std::vector<Node> parseForbiddenNodes(const nlohmann::json& config) {
    std::vector<Node> forbiddenNodes;
    if (!config.contains("forbidden_list")) return forbiddenNodes;
    for (const auto& forbidden : config["forbidden_list"]) {
        if (forbidden.contains("word") && forbidden.contains("position")) {
            forbiddenNodes.emplace_back(forbidden["word"], forbidden["position"]);
        }
    }
    return forbiddenNodes;
}
