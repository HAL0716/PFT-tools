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

std::string genDirPath(const Config& config) {
    auto getRootPath = []() -> std::filesystem::path {
        return std::filesystem::current_path().parent_path();
    };

    std::filesystem::path projectRoot = getRootPath();

    std::ostringstream basePath;
    basePath << projectRoot.string() << "/" << config.output.directory << "/";

    if (config.mode == "all-patterns") {
        basePath << config.alphabet_size << "-" << config.period << "-"
                 << config.forbidden_word_length.value_or(0);

        if (config.forbidden_per_position) {
            basePath << ":";
            const auto& forbiddenCounts = config.forbidden_per_position.value();
            for (size_t i = 0; i < forbiddenCounts.size(); ++i) {
                if (i > 0)
                    basePath << "-";
                basePath << forbiddenCounts[i];
            }
        }
    } else {
        basePath << config.mode;
    }

    return basePath.str();
}

std::string genFilePath(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                        const std::string& subDirectory) {
    std::string dirPath = baseDirectory + "/" + subDirectory + "/";
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
        std::cout << "Created directory: " << dirPath << std::endl;
    }

    std::ostringstream oss;
    oss << dirPath;
    for (size_t i = 0; i < forbiddenNodes.size(); ++i) {
        if (i > 0)
            oss << "-";
        const auto& node = forbiddenNodes[i];
        oss << node.getLabel() << ":" << node.getPhase();
    }
    oss << ".csv";

    return oss.str();
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

std::string getFileName(const std::string& filePath) {
    std::filesystem::path path(filePath);
    return path.filename().string();
}

}  // namespace path
