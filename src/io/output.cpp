#include "output.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>

std::string formatEdges(const std::vector<Edge>& edges) {
    std::ostringstream oss;

    // ヘッダー行を追加
    oss << "Source,Target,Label\n";
    // 各エッジをCSV形式で追加
    for (const auto& edge : edges) {
        oss << edge.getSource() << "," << edge.getTarget() << "," << edge.getLabel() << "\n";
    }

    return oss.str();
}

std::string formatAdjacencyMatrix(const std::vector<Node>& nodes, const std::vector<Edge>& edges) {
    size_t n = nodes.size();
    // ノードのラベルをインデックスにマッピング
    std::unordered_map<Node, size_t> toIdx;
    for (size_t i = 0; i < n; ++i) {
        toIdx[nodes[i]] = i;
    }

    // 隣接行列を初期化
    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n, 0));

    // エッジ情報を基に隣接行列を埋める
    for (const auto& edge : edges) {
        size_t srcIdx = toIdx[edge.getSource()];
        size_t tgtIdx = toIdx[edge.getTarget()];
        adjMatrix[srcIdx][tgtIdx] += 1;
    }

    // CSV形式の文字列に変換
    std::ostringstream oss;

    // 各行を追加
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (j > 0) oss << ",";
            oss << adjMatrix[i][j];
        }
        oss << "\n";
    }

    return oss.str();
}

void writeCSV(const std::string& filePath, const std::string& csvData) {
    std::ofstream outFile(filePath);

    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    outFile << csvData;
    outFile.close();

    std::cout << "Saved edges: " << filePath << std::endl;
}

std::filesystem::path getRootPath() {
    return std::filesystem::current_path().parent_path();
}

std::string genOutputPath(const json& config) {
    std::filesystem::path projectRoot = getRootPath();

    std::ostringstream basePath;
    basePath << projectRoot.string() << "/" << config["output"]["directory"].get<std::string>() << "/";

    if (config["mode"] == "all-patterns") {
        basePath << config["alphabet_size"].get<unsigned int>() << "-"
                 << config["period"].get<unsigned int>() << "-"
                 << config["forbidden_word_length"].get<unsigned int>();

        if (config.contains("forbidden_per_position")) {
            basePath << "-(";
            const auto& forbiddenCounts = config["forbidden_per_position"].get<std::vector<json>>();
            for (size_t i = 0; i < forbiddenCounts.size(); ++i) {
                if (i > 0) basePath << "-";
                basePath << forbiddenCounts[i].get<unsigned int>();
            }
            basePath << ")";
        }
    } else {
        basePath << config["mode"].get<std::string>();
    }

    return basePath.str();
}

void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes, const std::vector<Edge>& edges) {
    std::string dirPath = baseDirectory + "/edges/";

    // ディレクトリを作成し、存在しない場合は作成
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
        std::cout << "Created directory: " << dirPath << std::endl;
    }

    // ファイル名を生成
    std::ostringstream oss;
    oss << dirPath;
    for (size_t i = 0; i < forbiddenNodes.size(); ++i) {
        if (i > 0) oss << "-";
        oss << forbiddenNodes[i];
    }
    oss << ".csv";
    std::string fileName = oss.str();

    // エッジをCSV形式に変換
    std::string csvData = formatEdges(edges);

    // ファイルに保存
    writeCSV(fileName, csvData);
}

void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes, const std::vector<Node>& nodes, const std::vector<Edge>& edges) {
    std::string dirPath = baseDirectory + "/adjacency_matrix/";

    // ディレクトリを作成し、存在しない場合は作成
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
        std::cout << "Created directory: " << dirPath << std::endl;
    }

    // ファイル名を生成
    std::ostringstream oss;
    oss << dirPath;
    for (size_t i = 0; i < forbiddenNodes.size(); ++i) {
        if (i > 0) oss << "-";
        oss << forbiddenNodes[i];
    }
    oss << ".csv";
    std::string fileName = oss.str();

    // 隣接行列をCSV形式に変換
    std::string csvData = formatAdjacencyMatrix(nodes, edges);

    // ファイルに保存
    writeCSV(fileName, csvData);
}
