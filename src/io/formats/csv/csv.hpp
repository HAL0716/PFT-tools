#pragma once

#include <string>
#include <vector>

namespace io::formats::csv {

// CSVデータを表す2次元ベクターの型エイリアス
using CsvData = std::vector<std::vector<std::string>>;

/**
 * @brief CSVファイルからデータを読み取り2次元ベクターに格納する
 *
 * @param filePath 読み込むCSVファイルのパス
 * @param csvData データを格納する2次元ベクターへの参照
 * @return true ファイルの読み込みに成功した場合
 * @return false ファイルの読み込みに失敗した場合
 */
bool read(const std::string& filePath, CsvData& csvData);

/**
 * @brief 2次元ベクターのデータをCSVファイルに書き込む
 *
 * @param filePath 書き込むCSVファイルのパス
 * @param csvData 書き込むデータを含む2次元ベクターへの参照
 * @return true ファイルの書き込みに成功した場合
 * @return false ファイルの書き込みに失敗した場合
 */
bool write(const std::string& filePath, const CsvData& csvData);

}  // namespace io::formats::csv
