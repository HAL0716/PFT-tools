#pragma once

#include <string>

#include "core/Graph.hpp"
#include "csv.hpp"

namespace io::formats::csv::format {

// CSVデータを表す2次元ベクターの型エイリアス
using CsvData = io::formats::csv::CsvData;

/**
 * @brief グラフのエッジをCSV形式のデータに変換する
 *
 * @param graph エッジを取得する対象のグラフ
 * @return data CSV形式のエッジデータ
 */
CsvData edges(const Graph& graph);

/**
 * @brief グラフの隣接行列をCSV形式のデータに変換する
 *
 * @param graph 隣接行列を作成する対象のグラフ
 * @return data CSV形式の隣接行列データ
 */
CsvData adjacencyMatrix(const Graph& graph);

}  // namespace io::formats::csv::format
