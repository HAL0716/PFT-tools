#include "path/PathUtils.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>

namespace path {

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

void genDirectory(const std::string& filePath) {
    std::filesystem::path path(filePath);

    if (path.has_extension()) {
        path = path.parent_path();
    }

    if (!path.empty() && !std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
        std::cout << "Created directory: " << path.string() << std::endl;
    }
}

std::string getDirectory(const std::string& filePath, int generations) {
    if (generations <= 0) {
        throw std::invalid_argument("Generations must be greater than 0");
    }

    std::filesystem::path path(filePath);

    for (int i = 0; i < generations; ++i) {
        if (path.has_parent_path()) {
            path = path.parent_path();
        } else {
            throw std::out_of_range("Requested generations exceed the root directory");
        }
    }

    return path.string();
}

std::string getFileName(const std::string& filePath, bool withExt) {
    std::filesystem::path path(filePath);
    return withExt ? path.filename().string() : path.stem().string();
}

}  // namespace path
