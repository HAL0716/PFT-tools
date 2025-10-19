#include "path/PathUtils.hpp"

#include <filesystem>
#include <iostream>

namespace path::utils {

bool matchesExt(const std::filesystem::path& filePath, const std::string& ext) {
    return ext.empty() || filePath.extension() == (ext[0] == '.' ? ext : "." + ext);
}

std::vector<std::string> getFiles(const std::string& dirPath, const std::string& ext) {
    std::vector<std::string> files;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && matchesExt(entry.path(), ext)) {
                files.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }

    return files;
}

void genDirectory(const std::string& filePath) {
    std::filesystem::path path(filePath);

    if (path.has_extension()) {
        path = path.parent_path();
    }

    try {
        if (!std::filesystem::exists(path)) {
            std::filesystem::create_directories(path);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error creating directory: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error creating directory: " << e.what() << std::endl;
    }
}

std::filesystem::path ascendDirectories(std::filesystem::path path, int depth) {
    if (depth < 0) {
        throw std::invalid_argument("Depth must be non-negative");
    }

    while (depth-- > 0) {
        if (!path.has_parent_path() || path == path.root_path()) {
            throw std::out_of_range("Depth exceeds the root directory");
        }
        path = path.parent_path();
    }
    return path;
}

std::string extractPath(const std::string& filePath, int depth, bool includeDir, bool includeFile,
                        bool includeExt) {
    if (depth < 0) {
        throw std::invalid_argument("Depth must be non-negative");
    }

    std::filesystem::path path(filePath), result;

    if (includeDir) {
        result = ascendDirectories(path.parent_path(), depth);
    }

    if (includeFile) {
        result /= includeExt ? path.filename() : path.stem();
    }

    return includeDir || includeFile ? result.string() : "";
}

}  // namespace path::utils
