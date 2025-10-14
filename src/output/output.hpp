#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "formatter.hpp"
#include "input/Config.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// 出力ディレクトリパスを生成する関数
std::string genOutputPath(const Config& config);

// エッジを指定されたディレクトリに保存する関数
void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const Graph& graph);

// 隣接行列を指定されたディレクトリに保存する関数
void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                         const Graph& graph);

namespace output_helpers {

std::string generateFilePath(const std::string& baseDirectory,
                             const std::vector<Node>& forbiddenNodes,
                             const std::string& subDirectory);

}  // namespace output_helpers
