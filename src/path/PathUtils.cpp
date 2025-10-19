#include "path/PathUtils.hpp"

#include <filesystem>
#include <iostream>

namespace path::utils {

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

std::filesystem::path ascendDirectories(std::filesystem::path path, int depth) {
    if (depth < 0) {
        throw std::invalid_argument("Depth must be non-negative");
    }

    for (int i = 0; i < depth; ++i) {
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

    // ディレクトリ部分を取得
    if (includeDir) {
        result = ascendDirectories(path.parent_path(), depth);
    }

    // ファイル名部分を取得
    if (includeFile) {
        result /= includeExt ? path.filename() : path.stem();
    }

    // 両方含めない場合は空文字列を返す
    if (!includeDir && !includeFile) {
        return "";
    }

    return result.string();
}

}  // namespace path::utils
