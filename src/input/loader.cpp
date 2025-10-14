#include "loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace loader {

bool json(const std::string& filePath, nlohmann::json& jsonData) {
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

bool csv(const std::string& filePath, std::vector<std::vector<std::string>>& csvData) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream lineStream(line);
        std::string cell;

        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        csvData.push_back(row);
    }

    return true;
}

} // namespace loader
