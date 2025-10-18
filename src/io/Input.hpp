#pragma once

#include <string>
#include <vector>

#include "Config.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"

namespace io::input {

using Config = io::type::Config;

// Config関連
bool readConfigJson(const std::string& filePath, Config& config);

// Edges関連
bool readEdgesCSV(const std::string& filePath, Graph& graph);

// Adjacency Matrix関連
bool readMatrixCSV(const std::string& filePath, Graph& graph);

// Configからノードリストを生成
std::vector<std::vector<Node>> genNodesFromConfig(const Config& config);

}  // namespace io::input
