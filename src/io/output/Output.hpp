#pragma once

#include <string>

#include "core/Graph.hpp"

namespace io::output {

/**
 * @brief エッジリストを指定されたファイルに保存する
 *
 * @param filePath 出力先ファイルのパス
 * @param graph 入力グラフオブジェクト
 * @return true 保存に成功した場合
 * @return false 保存に失敗した場合
 */
bool edges(const std::string& filePath, const Graph& graph);

/**
 * @brief 隣接行列を指定されたファイルに保存する
 *
 * @param filePath 出力先ファイルのパス
 * @param graph 入力グラフオブジェクト
 * @return true 保存に成功した場合
 * @return false 保存に失敗した場合
 */
bool adjacencyMatrix(const std::string& filePath, const Graph& graph);

/**
 * @brief 許可されたシーケンスを指定されたファイルに保存する
 *
 * @param filePath 出力先ファイルのパス
 * @param graph 入力グラフオブジェクト
 * @param length シーケンスの長さ
 * @return true 保存に成功した場合
 * @return false 保存に失敗した場合
 */
bool sequences(const std::string& filePath, const Graph& graph, unsigned int length);

}  // namespace io::output
