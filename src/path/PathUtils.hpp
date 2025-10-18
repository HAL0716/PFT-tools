#pragma once

#include <string>
#include <vector>

#include "core/Node.hpp"
#include "io/Config.hpp"

namespace path {

using Config = io::formats::json::Config;

// 指定されたディレクトリ内のすべてのCSVファイルを取得する関数
std::vector<std::string> getCsvFiles(const std::string& directoryPath);

// ディレクトリパスを生成する関数
std::string genDirPath(const Config& config);

// ファイルパスを生成する関数
std::string genFilePath(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                        const std::string& subDirectory, const std::string& extension = "csv");

// 指定されたファイルパスからN世代前の親ディレクトリを取得する関数
std::string getDirectory(const std::string& filePath, int generations = 1);

// 指定されたファイルパスからファイル名を取得する関数
std::string getFileName(const std::string& filePath, bool withExt = true);

// 指定されたディレクトリが存在しない場合は作成する関数
void genDirectory(const std::string& filePath);

}  // namespace path
