#include "csv.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace csv {

bool read(const std::string& filePath, std::vector<std::vector<std::string>>& csvData) {
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

bool write(const std::string& filePath, const std::vector<std::vector<std::string>>& csvData) {
    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
        return false;
    }

    for (const auto& row : csvData) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i > 0)
                file << ",";
            file << row[i];
        }
        file << "\n";
    }

    return true;
}

}  // namespace csv
