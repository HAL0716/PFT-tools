#include "Output.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "../formats/csv/format.hpp"
#include "path/PathUtils.hpp"

void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const Graph& graph) {
    std::string filePath = path::genFilePath(baseDirectory, forbiddenNodes, "edges");

    const auto& csvData = io::formats::csv::format::edges(graph);

    if (io::formats::csv::write(filePath, csvData)) {
        std::cout << "Saved file: " << filePath << std::endl;
    } else {
        throw std::ios_base::failure("Failed to write CSV data to file: " + filePath);
    }
}

void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                         const Graph& graph) {
    std::string filePath = path::genFilePath(baseDirectory, forbiddenNodes, "adjacency_matrix");

    const auto& csvData = io::formats::csv::format::adjacencyMatrix(graph);

    if (io::formats::csv::write(filePath, csvData)) {
        std::cout << "Saved file: " << filePath << std::endl;
    } else {
        throw std::ios_base::failure("Failed to write CSV data to file: " + filePath);
    }
}

// エッジラベル系列を指定されたディレクトリに保存する関数
void saveSequences(const std::string& filePath, const std::unordered_set<std::string>& sequences) {
    // ソートして保存
    std::vector<std::string> sortedSequences(sequences.begin(), sequences.end());
    std::sort(sortedSequences.begin(), sortedSequences.end());

    std::string csvData;
    for (const auto& sequence : sortedSequences) {
        csvData += sequence + "\n";
    }

    path::genDirectory(filePath);
    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::ios_base::failure("Failed to open file for writing: " + filePath);
    }
    outFile << csvData;
    outFile.close();
    std::cout << "Saved file: " << filePath << std::endl;
}
