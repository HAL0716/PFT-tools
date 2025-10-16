#pragma once

#include <string>

#include "core/Graph.hpp"

namespace graphviz {

// ダミーの.dotファイルを保存する関数
void saveDotFile(const std::string& filePath, const Graph& graph);

// 頂点の定義を出力する関数
void writeVertices(std::ostringstream& content);

// 辺の定義を出力する関数
void writeEdges(std::ostringstream& content);

// グラフの内容を生成する関数
std::string generateGraphContent(const Graph& graph);

// DOTファイルをTeXファイルに変換する関数
void cvtDot2Tex(const std::string& dotFilePath, const std::string& texFilePath);

}  // namespace graphviz
