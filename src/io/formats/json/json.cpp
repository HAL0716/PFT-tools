#include "json.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace json {

bool read(const std::string& filePath, nlohmann::json& jsonData) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return false;
    }
    try {
        file >> jsonData;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse JSON file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool write(const std::string& filePath, const nlohmann::json& jsonData) {
    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
        return false;
    }
    try {
        file << jsonData.dump(4);
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to write JSON file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

}  // namespace json
