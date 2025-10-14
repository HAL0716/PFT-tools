#pragma once

#include <string>
#include <vector>

#include "core/Edge.hpp"
#include "core/Node.hpp"

namespace writer {
// CSV形式の文字列をファイルに保存する関数
void writeCSV(const std::string& filePath, const std::string& csvData);

// エッジを指定されたディレクトリに保存する関数
void edges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
           const std::vector<Edge>& edges);

// 隣接行列を指定されたディレクトリに保存する関数
void adjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                     const std::vector<Node>& nodes, const std::vector<Edge>& edges);
}  // namespace writer
