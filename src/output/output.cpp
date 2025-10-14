#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "core/Graph.hpp"
#include "formatter.hpp"
#include "input/Config.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string genOutputPath(const Config& config) {
    auto getRootPath = []() -> std::filesystem::path {
        return std::filesystem::current_path().parent_path();
    };

    std::filesystem::path projectRoot = getRootPath();

    std::ostringstream basePath;
    basePath << projectRoot.string() << "/" << config.output.directory << "/";

    if (config.mode == "all-patterns") {
        basePath << config.alphabet_size << "-" << config.period << "-"
                 << config.forbidden_word_length.value_or(0);

        if (config.forbidden_per_position) {
            basePath << ":";
            const auto& forbiddenCounts = config.forbidden_per_position.value();
            for (size_t i = 0; i < forbiddenCounts.size(); ++i) {
                if (i > 0)
                    basePath << "-";
                basePath << forbiddenCounts[i];
            }
        }
    } else {
        basePath << config.mode;
    }

    return basePath.str();
}

namespace output_helpers {

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

}  // namespace output_helpers

void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const Graph& graph) {
    using namespace output_helpers;

    std::string filePath = generateFilePath(baseDirectory, forbiddenNodes, "edges");

    std::string csvData = formatEdgesCSV(graph);

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
    using namespace output_helpers;

    std::string filePath = generateFilePath(baseDirectory, forbiddenNodes, "adjacency_matrix");

    std::string csvData = formatAdjacencyMatrixCSV(graph);

    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::ios_base::failure("Failed to open file for writing: " + filePath);
    }
    outFile << csvData;
    outFile.close();
    std::cout << "Saved file: " << filePath << std::endl;
}
