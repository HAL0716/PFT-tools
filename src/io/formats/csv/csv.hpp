#pragma once

#include <string>
#include <vector>

#include "io/formats/csv/format.hpp"

namespace csv {

// CSVファイルを読み込む関数
bool read(const std::string& filePath, std::vector<std::vector<std::string>>& csvData);

// CSVファイルを書き込む関数
bool write(const std::string& filePath, const std::vector<std::vector<std::string>>& csvData);

}  // namespace csv
