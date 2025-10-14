#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace loader {
// JSONデータをファイルから読み込む関数
bool json(const std::string& filePath, nlohmann::json& jsonData);

// CSVデータを2次元ベクターに読み込む関数
bool csv(const std::string& filePath, std::vector<std::vector<std::string>>& csvData);

// ディレクトリ内のすべての.csvファイルを取得する関数
std::vector<std::string> getCsvFiles(const std::string& directoryPath);
}  // namespace loader
