#include "loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

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

std::vector<std::string> getCsvFiles(const std::string& directoryPath) {
    std::vector<std::string> csvFiles;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                csvFiles.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to read directory: " << e.what() << std::endl;
    }

    return csvFiles;
}

} // namespace loader
