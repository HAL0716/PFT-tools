#include "format.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace io::formats::csv::format {

CsvData edges(const Graph& graph) {
    // CSVデータ形式に変換
    CsvData csvData;
    for (const auto& edge : graph.getEdges(Graph::mode::ID)) {
        csvData.push_back(
            {edge.getSource().getLabel(), edge.getTarget().getLabel(), edge.getLabel()});
    }

    return csvData;
}

CsvData adjacencyMatrix(const Graph& graph) {
    // ノード数を取得
    auto n = graph.getNodes().size();

    // 隣接行列を初期化
    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n, 0));
    for (const auto& edge : graph.getEdges(Graph::mode::ID)) {
        try {
            auto srcIdx = std::stoi(edge.getSource().getLabel());
            auto tgtIdx = std::stoi(edge.getTarget().getLabel());
            adjMatrix[srcIdx][tgtIdx] += 1;
        } catch (const std::exception& e) {
            throw std::invalid_argument("Node labels must be valid integers for ID mode.");
        }
    }

    // CSVデータ形式に変換
    CsvData csvData(n, std::vector<std::string>(n, ""));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            csvData[i][j] = std::to_string(adjMatrix[i][j]);
        }
    }

    return csvData;
}

CsvData sequences(const Graph& graph, unsigned int length) {
    // エッジラベルシーケンスを取得
    auto sequencesSet = graph.getEdgeLabelSequences(length);

    // CSVデータ形式に変換
    CsvData csvData;
    for (const auto& sequence : sequencesSet) {
        csvData.push_back({sequence});
    }

    return csvData;
}

}  // namespace io::formats::csv::format
