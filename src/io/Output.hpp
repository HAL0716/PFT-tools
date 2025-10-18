#pragma once

#include <string>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "nlohmann/json.hpp"

using CsvData = std::vector<std::vector<std::string>>;
using json = nlohmann::json;

// ユーティリティ関数
bool write(const std::string& path, const std::string& data);
bool exec(const std::string& cmd);

// CSV関連
bool writeCsv(const std::string& filePath, const CsvData& csvData);
bool writeEdgesCsv(const std::string& filePath, const Graph& graph);
bool writeMatrixCsv(const std::string& filePath, const Graph& graph);
bool writeSeqCsv(const std::string& filePath, const Graph& graph, unsigned int length);

// JSON関連
bool writeJson(const std::string& filePath, const json& jsonData);

// Graphviz関連
bool writeDot(const std::string& path, const Graph& graph);
bool dotToTeX(const std::string& dir, const std::vector<Node>& nodes);
bool texToPDF(const std::string& dir, const std::vector<Node>& nodes);
bool pdfToPNG(const std::string& dir, const std::vector<Node>& nodes);
