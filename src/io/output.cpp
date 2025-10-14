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
    // ディレクトリを作成し、存在しない場合は作成
    if (!std::filesystem::exists(baseDirectory)) {
        std::filesystem::create_directories(baseDirectory);
        std::cout << "Created directory: " << baseDirectory << std::endl;
    }

    // ファイル名を生成
    std::ostringstream oss;
    oss << baseDirectory << "/";
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
