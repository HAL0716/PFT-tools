#include "Input.hpp"

#include <iostream>
#include <unordered_set>
#include <vector>

#include "../formats/csv/csv.hpp"
#include "../formats/json/json.hpp"

bool loadConfig(const std::string& filePath, Config& config) {
    io::formats::json::json jsonData;
    if (!io::formats::json::read(filePath, jsonData)) {
        return false;
    }

    try {
        config = jsonData.get<Config>();
        config.validate();

        if (config.mode == "custom" && config.algorithm == "DeBruijn") {
            config.formatForDeBruijn();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid config: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool loadEdges(const std::string& filePath, Graph& graph) {
    std::vector<std::vector<std::string>> csvData;
    if (!io::formats::csv::read(filePath, csvData)) {
        return false;
    }

    std::unordered_set<Node> nodes;

    for (const auto& row : csvData) {
        if (row.size() < 3) {
            std::cerr << "Error: Invalid edge data format in file: " << filePath << std::endl;
            return false;
        }

        Node source(row[0]);
        Node target(row[1]);
        nodes.insert(source);
        nodes.insert(target);
        std::string label = row[2];
        graph.addEdge(Edge(source, target, label));
    }

    for (const auto& node : nodes) {
        graph.addNode(node);
    }

    return true;
}

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph) {
    std::vector<std::vector<std::string>> csvData;
    if (!io::formats::csv::read(filePath, csvData)) {
        return false;
    }

    for (size_t i = 0; i < csvData.size(); ++i) {
        if (csvData[i].size() != csvData.size()) {
            std::cerr << "Error: Adjacency matrix must be square in file: " << filePath
                      << std::endl;
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
