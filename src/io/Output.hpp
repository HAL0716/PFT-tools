#pragma once

#include <string>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "nlohmann/json.hpp"

namespace io::output {

using CsvData = std::vector<std::vector<std::string>>;
using json = nlohmann::json;

// CSV関連
bool writeEdgesCsv(const std::string& filePath, const Graph& graph);
bool writeMatrixCsv(const std::string& filePath, const Graph& graph);
bool writeSeqCsv(const std::string& filePath, const Graph& graph, unsigned int length);

// Graphviz関連
bool writeDot(const std::string& filePath, const Graph& graph);
bool dotToTeX(const std::string& dotFilePath, const std::string& texFilePath);
bool texToPDF(const std::string& texFilePath, const std::string& pdfFilePath);
bool pdfToPNG(const std::string& pdfFilePath, const std::string& pngFilePath);

}  // namespace io::output
