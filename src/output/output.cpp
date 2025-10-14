#include <filesystem>
#include <sstream>

#include "formatter.hpp"
#include "input/Config.hpp"
#include "input/loader.hpp"
#include "nlohmann/json.hpp"
#include "writer.hpp"

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
            basePath << "-(";
            const auto& forbiddenCounts = config.forbidden_per_position.value();
            for (size_t i = 0; i < forbiddenCounts.size(); ++i) {
                if (i > 0)
                    basePath << "-";
                basePath << forbiddenCounts[i];
            }
            basePath << ")";
        }
    } else {
        basePath << config.mode;
    }

    return basePath.str();
}

void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const std::vector<Edge>& edges) {
    // フォーマット処理
    std::string formattedEdges = formatter::edges(edges);

    // 保存処理
    writer::edges(baseDirectory, forbiddenNodes, edges);
}

void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                         const std::vector<Node>& nodes, const std::vector<Edge>& edges) {
    // フォーマット処理
    std::string formattedMatrix = formatter::adjacencyMatrix(nodes, edges);

    // 保存処理
    writer::adjacencyMatrix(baseDirectory, forbiddenNodes, nodes, edges);
}
