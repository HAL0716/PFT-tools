#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

namespace loader {
    // JSONデータをファイルから読み込む関数
    bool json(const std::string& filePath, nlohmann::json& jsonData);

    // CSVデータを2次元ベクターに読み込む関数
    bool csv(const std::string& filePath, std::vector<std::vector<std::string>>& csvData);
}
