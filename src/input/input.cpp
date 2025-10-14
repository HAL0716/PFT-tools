#include "input.hpp"
#include "loader.hpp"
#include <iostream>
#include "core/Edge.hpp"
#include "core/Node.hpp"

bool loadConfig(const std::string& configPath, nlohmann::json& config) {
    return loader::json(configPath, config);
}

bool loadEdges(const std::string& filePath, Graph& graph) {
    std::vector<std::vector<std::string>> csvData;
    if (!loader::csv(filePath, csvData)) {
        return false;
    }

    for (const auto& row : csvData) {
        if (row.size() < 2) {
            std::cerr << "Error: Invalid edge data format in file: " << filePath << std::endl;
            return false;
        }
        Node source(row[0]);
        Node target(row[1]);
        graph.addEdge(Edge(source, target));
    }

    return true;
}

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph) {
    std::vector<std::vector<std::string>> csvData;
    if (!loader::csv(filePath, csvData)) {
        return false;
    }

    for (size_t i = 0; i < csvData.size(); ++i) {
        Node source(std::to_string(i));
        graph.addNode(source);
        for (size_t j = 0; j < csvData[i].size(); ++j) {
            if (csvData[i][j] == "1") {
                Node target(std::to_string(j));
                graph.addEdge(Edge(source, target));
            }
        }
    }

    return true;
}
