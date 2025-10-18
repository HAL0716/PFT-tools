#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "../formats/json/Config.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// エッジを指定されたディレクトリに保存する関数
void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const Graph& graph);

// 隣接行列を指定されたディレクトリに保存する関数
void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                         const Graph& graph);

// エッジラベル系列を指定されたディレクトリに保存する関数
void saveSequences(const std::string& filePath, const std::unordered_set<std::string>& sequences);
