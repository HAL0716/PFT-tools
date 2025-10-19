#include "Input.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "io/utils.hpp"
#include "nlohmann/json.hpp"
#include "utils/CombinationUtils.hpp"

namespace io::input {

// CSVファイルを読み込む関数
bool readCsv(const std::string& filePath, std::vector<std::vector<std::string>>& csvData) {
    std::ifstream file(filePath);
    if (!io::utils::checkFileOpen(file, filePath)) {
        std::cerr << "Failed to open CSV file: " << filePath << std::endl;
        return false;
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
        std::cerr << "Failed to open JSON file: " << filePath << std::endl;
        return false;
    }
    try {
        file >> jsonData;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON file: " << filePath << " - " << e.what() << std::endl;
        return false;
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

std::vector<std::vector<std::vector<Node>>> genForbiddenList(const Config& config) {
    std::vector<std::vector<std::vector<Node>>> forbiddenNodesList;
    const auto& words = combine(ALPHABET.substr(0, config.alphabet_size),
                                config.forbidden_word_length.value(), true);
    const auto& perPosition = config.forbidden_per_position.value();
    if (perPosition.size() < config.period) {
        io::utils::printErrorAndExit(
            "forbidden_per_position.size() (" + std::to_string(perPosition.size()) +
            ") is less than config.period (" + std::to_string(config.period) +
            "). Check your configuration to avoid truncation.");
    }
    unsigned int period = std::min(config.period, static_cast<unsigned int>(perPosition.size()));

    for (unsigned int p = 0; p < period; ++p) {
        unsigned int n = perPosition[p];
        if (n > words.size()) {
            io::utils::printErrorAndExit(
                "forbidden_per_position value exceeds total combinations.");
        }

        std::vector<Node> forbiddenNodes;
        for (const auto& w : words) {
            forbiddenNodes.emplace_back(w, p);
        }

        // 組み合わせを生成
        forbiddenNodesList.emplace_back(n == 0 ? std::vector<std::vector<Node>>()
                                        : n == words.size()
                                            ? std::vector<std::vector<Node>>{forbiddenNodes}
                                            : combine(forbiddenNodes, n, false));
    }

    return forbiddenNodesList;
}

std::vector<std::vector<Node>> combineNodes(
    const std::vector<std::vector<std::vector<Node>>>& forbiddenList) {
    std::vector<std::vector<Node>> combinedNodes;
    std::function<void(unsigned int, std::vector<Node>&)> dfs = [&](unsigned int depth,
                                                                    std::vector<Node>& current) {
        if (depth == forbiddenList.size()) {
            combinedNodes.push_back(std::move(current));
            return;
        }
        for (const auto& nodes : forbiddenList[depth]) {
            std::vector<Node> next = current;
            next.insert(next.end(), nodes.begin(), nodes.end());
            dfs(depth + 1, next);
        }
    };
    std::vector<Node> initial;
    dfs(0, initial);
    return combinedNodes;
}

std::vector<std::vector<Node>> genNodesFromConfig(const Config& config) {
    if (config.mode == "custom") {
        std::vector<Node> forbiddenNodes;
        for (const auto& forbidden : config.forbidden_words.value()) {
            forbiddenNodes.emplace_back(forbidden.label, forbidden.phase);
        }
        if (forbiddenNodes.empty()) {
            io::utils::printErrorAndExit("forbidden_words is empty.");
        }
        return {std::move(forbiddenNodes)};
    } else if (config.mode == "all-patterns") {
        auto forbiddenNodesList = genForbiddenList(config);
        return combineNodes(forbiddenNodesList);
    } else {
        io::utils::printErrorAndExit("Unknown mode '" + config.mode + "'.");
    }
    return {};
}

}  // namespace io::input
