#pragma once

#include <string>
#include <vector>

#include "core/Node.hpp"
#include "io/Config.hpp"

namespace path {

using Config = io::type::Config;

// ディレクトリパスを生成する関数
std::string genDirPath(const Config& config);

// ファイルパスを生成する関数
std::string genFilePath(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                        const std::string& subDirectory, const std::string& extension = "csv");

// 指定されたディレクトリが存在しない場合は作成する関数
void genDirectory(const std::string& filePath);

}  // namespace path
