#include "input.hpp"
#include "loader.hpp"
#include <iostream>
#include "core/Edge.hpp"
#include "core/Node.hpp"

bool loadConfig(const std::string& filePath, Config& config) {
    nlohmann::json jsonConfig;
    if (!loader::json(filePath, jsonConfig)) {
        return false;
    }

    try {
        config = jsonConfig.get<Config>();
        config.validate();
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid config: " << e.what() << std::endl;
        return false;
    }

    return true;
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
        if (csvData[i].size() != csvData.size()) {
            std::cerr << "Error: Adjacency matrix must be square in file: " << filePath << std::endl;
            return false;
        }

        Node source(std::to_string(i));
        graph.addNode(source);
        for (size_t j = 0; j < csvData[i].size(); ++j) {
            for (size_t n = 0; n < std::stoi(csvData[i][j]); ++n) {
                Node target(std::to_string(j));
                graph.addEdge(Edge(source, target, std::to_string(n)));
            }
        }
    }

    return true;
}
