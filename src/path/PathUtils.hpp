#pragma once

#include <string>
#include <vector>

namespace path::utils {

// 指定されたディレクトリ内のすべてのCSVファイルを取得する関数
std::vector<std::string> getCsvFiles(const std::string& directoryPath);

// 汎用的なパス抽出関数
std::string extractPath(const std::string& filePath, int depth = 0, bool includeDir = true,
                        bool includeFile = true, bool includeExt = true);

// 指定されたディレクトリが存在しない場合は作成する関数
void genDirectory(const std::string& filePath);

}  // namespace path::utils
