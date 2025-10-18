#include "io/formats/csv/csv.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "io/utils/utils.hpp"

namespace io::formats::csv {

bool read(const std::string& filePath, CsvData& csvData) {
    std::ifstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
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

bool write(const std::string& filePath, const CsvData& csvData) {
    std::ofstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
        return false;
    }

    for (const auto& row : csvData) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i > 0) {
                file << ",";
            }
            file << row[i];
        }
        file << "\n";
    }

    return true;
}

}  // namespace io::formats::csv
