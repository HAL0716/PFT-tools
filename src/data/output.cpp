#include "output.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "csv/format.hpp"
#include "path/PathUtils.hpp"

void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const Graph& graph) {
    std::string filePath = path::genFilePath(baseDirectory, forbiddenNodes, "edges");

    std::string csvData = format::edges(graph);

    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::ios_base::failure("Failed to open file for writing: " + filePath);
    }
    outFile << csvData;
    outFile.close();
    std::cout << "Saved file: " << filePath << std::endl;
}

void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                         const Graph& graph) {
    std::string filePath = path::genFilePath(baseDirectory, forbiddenNodes, "adjacency_matrix");

    std::string csvData = format::adjacencyMatrix(graph);

    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::ios_base::failure("Failed to open file for writing: " + filePath);
    }
    outFile << csvData;
    outFile.close();
    std::cout << "Saved file: " << filePath << std::endl;
}
