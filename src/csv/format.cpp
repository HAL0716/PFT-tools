#include "format.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>

namespace format {

std::string generateFilePath(const std::string& baseDirectory,
                             const std::vector<Node>& forbiddenNodes,
                             const std::string& subDirectory) {
    std::string dirPath = baseDirectory + "/" + subDirectory + "/";
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
        std::cout << "Created directory: " << dirPath << std::endl;
    }

    std::ostringstream oss;
    oss << dirPath;
    for (size_t i = 0; i < forbiddenNodes.size(); ++i) {
        if (i > 0)
            oss << "-";
        oss << forbiddenNodes[i];
    }
    oss << ".csv";

    return oss.str();
}

std::string edges(const Graph& graph) {
    auto nodes = graph.getNodes();
    std::unordered_map<Node, size_t> toIdx;
    for (size_t i = 0; i < nodes.size(); ++i) {
        toIdx[nodes[i]] = i;
    }

    std::ostringstream csvData;
    for (const auto& edge : graph.getEdges()) {
        size_t srcIdx = toIdx.at(edge.getSource());
        size_t tgtIdx = toIdx.at(edge.getTarget());
        csvData << srcIdx << "," << tgtIdx << "," << edge.getLabel() << "\n";
    }
    return csvData.str();
}

std::string adjacencyMatrix(const Graph& graph) {
    auto nodes = graph.getNodes();
    std::unordered_map<Node, size_t> toIdx;
    for (size_t i = 0; i < nodes.size(); ++i) {
        toIdx[nodes[i]] = i;
    }

    std::vector<std::vector<int>> adjMatrix(nodes.size(), std::vector<int>(nodes.size(), 0));
    for (const auto& edge : graph.getEdges()) {
        size_t srcIdx = toIdx.at(edge.getSource());
        size_t tgtIdx = toIdx.at(edge.getTarget());
        adjMatrix[srcIdx][tgtIdx] += 1;
    }

    std::ostringstream csvData;
    for (const auto& row : adjMatrix) {
        for (size_t j = 0; j < row.size(); ++j) {
            if (j > 0)
                csvData << ",";
            csvData << row[j];
        }
        csvData << "\n";
    }
    return csvData.str();
}

}  // namespace format
