#include "Input.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "io/utils.hpp"
#include "nlohmann/json.hpp"

// CSVファイルを読み込む関数
bool readCsv(const std::string& filePath, std::vector<std::vector<std::string>>& csvData) {
    std::ifstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
        io::utils::printErrorAndExit("Failed to open CSV file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        csvData.push_back(row);
    }

    return true;
}

// JSONファイルを読み込む関数
bool readJson(const std::string& filePath, nlohmann::json& jsonData) {
    std::ifstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
        io::utils::printErrorAndExit("Failed to open JSON file: " + filePath);
    }
    try {
        file >> jsonData;
    } catch (const std::exception& e) {
        io::utils::printErrorAndExit("Failed to parse JSON file: " + filePath + " - " + e.what());
    }

    return true;
}

// Config関連
bool readConfigJson(const std::string& filePath, Config& config) {
    nlohmann::json jsonData;
    if (!readJson(filePath, jsonData)) {
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

// Edges関連
bool readEdgesCSV(const std::string& filePath, Graph& graph) {
    std::vector<std::vector<std::string>> csvData;
    if (!readCsv(filePath, csvData)) {
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

// Adjacency Matrix関連
bool readMatrixCSV(const std::string& filePath, Graph& graph) {
    std::vector<std::vector<std::string>> csvData;
    if (!readCsv(filePath, csvData)) {
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
