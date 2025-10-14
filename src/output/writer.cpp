#include "writer.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "formatter.hpp"

namespace writer {

void writeCSV(const std::string& filePath, const std::string& csvData) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }
    outFile << csvData;
    outFile.close();
    std::cout << "Saved file: " << filePath << std::endl;
}

void edges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
           const std::vector<Edge>& edges) {
    std::string dirPath = baseDirectory + "/edges/";
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
    std::string fileName = oss.str();

    std::string csvData = formatter::edges(edges);
    writeCSV(fileName, csvData);
}

void adjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                     const std::vector<Node>& nodes, const std::vector<Edge>& edges) {
    std::string dirPath = baseDirectory + "/adjacency_matrix/";
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
    std::string fileName = oss.str();

    std::string csvData = formatter::adjacencyMatrix(nodes, edges);
    writeCSV(fileName, csvData);
}

}  // namespace writer
