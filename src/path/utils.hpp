#pragma once

#include <string>
#include <vector>

namespace path::utils {

// 汎用的なファイル取得関数
std::vector<std::string> getFiles(const std::string& dirPath, const std::string& ext = "");

// 汎用的なパス抽出関数
std::string extractPath(const std::string& filePath, int depth = 0, bool includeDir = true,
                        bool includeFile = true, bool includeExt = true);

// 指定されたディレクトリが存在しない場合は作成する関数
void genDir(const std::string& filePath);

}  // namespace path::utils
