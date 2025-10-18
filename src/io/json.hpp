#pragma once

#include <string>

#include "nlohmann/json.hpp"

namespace io::formats::json {

using json = nlohmann::json;

/**
 * @brief JSONファイルを読み込み、データを取得する
 *
 * @param filePath 読み込むJSONファイルのパス
 * @param jsonData 読み込んだデータを格納するnlohmann::jsonオブジェクト
 * @return true ファイルの読み込みに成功した場合
 * @return false ファイルの読み込みに失敗した場合
 */
bool read(const std::string& filePath, json& jsonData);

/**
 * @brief JSONデータをファイルに書き込む
 *
 * @param filePath 書き込むJSONファイルのパス
 * @param jsonData 書き込むnlohmann::jsonオブジェクト
 * @return true ファイルの書き込みに成功した場合
 * @return false ファイルの書き込みに失敗した場合
 */
bool write(const std::string& filePath, const json& jsonData);

}  // namespace io::formats::json
