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
bool writePdf(const std::string& filePath, const Graph& graph);
bool writePng(const std::string& filePath, const Graph& graph);

template <typename PathGen, typename WriteFunc>
bool writeGraph(const std::string& type, const std::string& ext, const PathGen& pathGen,
                WriteFunc writeFunc, const Graph& graph) {
    const std::string filePath = pathGen(type, ext);
    return writeFunc(filePath, graph);
}

}  // namespace io::output
