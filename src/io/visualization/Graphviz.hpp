#pragma once

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"

namespace graphviz {

// ヘルパー関数
std::unordered_map<Node, size_t> genNodeIndexMap(const Graph& graph);

// DOTファイル関連の関数
bool saveDot(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
             const Graph& graph);
std::string cvtGraph2Dot(const Graph& graph);

// ファイル変換関連の関数
bool executeCommand(const std::string& command, const std::string& errorMessage);
bool cvtDot2TeX(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes);
bool cvtTex2PDF(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes);
bool cvtPDF2PNG(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes);

}  // namespace graphviz
