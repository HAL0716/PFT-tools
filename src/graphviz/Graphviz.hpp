#pragma once

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"

namespace graphviz {

// ヘルパー関数
std::unordered_map<Node, size_t> generateNodeIndexMap(const Graph& graph);

// DOTファイル関連の関数
void saveDot(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
             const Graph& graph);
std::string cvtGraph2Dot(const Graph& graph);
void writeHeader(std::ostringstream& content);
void writeVertices(std::ostringstream& content, const Graph& graph);
void writeEdges(std::ostringstream& content, const Graph& graph);

// ファイル変換関連の関数
void executeCommand(const std::string& command, const std::string& errorMessage);
void cvtDot2TeX(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes);
void cvtTex2PDF(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes);
void cvtPDF2PNG(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes);

}  // namespace graphviz
