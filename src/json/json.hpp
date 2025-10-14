#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace json {

// JSONファイルを読み込む関数
bool read(const std::string& filePath, nlohmann::json& jsonData);

// JSONファイルを書き込む関数
bool write(const std::string& filePath, const nlohmann::json& jsonData);

}  // namespace json
