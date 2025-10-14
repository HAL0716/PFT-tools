#pragma once

#include <string>
#include <vector>

#include "core/Node.hpp"
#include "json/Config.hpp"

namespace path {

// 指定されたディレクトリ内のすべてのCSVファイルを取得する関数
std::vector<std::string> getCsvFiles(const std::string& directoryPath);

// ディレクトリパスを生成する関数
std::string genDirPath(const Config& config);

// ファイルパスを生成する関数
std::string genFilePath(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                        const std::string& subDirectory);

}  // namespace path
