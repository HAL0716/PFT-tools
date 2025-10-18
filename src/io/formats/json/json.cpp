#include "json.hpp"

#include <fstream>
#include <iostream>

#include "io/utils/utils.hpp"

namespace io::formats::json {

bool read(const std::string& filePath, json& jsonData) {
    std::ifstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
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

bool write(const std::string& filePath, const json& jsonData) {
    std::ofstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
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

}  // namespace io::formats::json
