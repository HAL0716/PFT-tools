#include <CLI/CLI.hpp>
#include <cstdlib>  // std::getenv
#include <iostream>
#include <map>      // std::map
#include <memory>   // std::unique_ptr
#include <numeric>  // std::transform_reduce
#include <string>

#include "algorithm/Beal.hpp"
#include "algorithm/DeBruijn.hpp"
#include "algorithm/Moore.hpp"
#include "analysis/eigenvalues.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "core/constants.hpp"
#include "data/input.hpp"
#include "data/output.hpp"
#include "graphviz/Graphviz.hpp"
#include "json/Config.hpp"
#include "path/PathUtils.hpp"
#include "utils/CombinationUtils.hpp"
#include "utils/GraphUtils.hpp"

using json = nlohmann::json;

// エラーメッセージを一元管理
void printErrorAndExit(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
}

// GraphGeneratorを生成する関数マップ
static const std::map<std::string, std::function<std::unique_ptr<GraphGenerator>(const Config&)>>
    generatorMap = {{"Beal",
                     [](const Config& config) {
                         return std::make_unique<Beal>(config.alphabet_size, config.period,
                                                       config.forbidden_word_length.value_or(0));
                     }},
                    {"DeBruijn", [](const Config& config) {
                         return std::make_unique<DeBruijn>(config.alphabet_size, config.period,
                                                           config.forbidden_word_length.value());
                     }}};

// 動的にGraphGeneratorの派生クラスを生成するファクトリ関数
std::unique_ptr<GraphGenerator> createGraphGenerator(const Config& config) {
    auto it = generatorMap.find(config.algorithm);

    if (it != generatorMap.end()) {
        return it->second(config);
    } else {
        throw std::invalid_argument("Unknown algorithm: " + config.algorithm);
    }
}

// forbiddenNodesListを生成する関数
std::vector<std::vector<std::vector<Node>>> generateForbiddenNodesList(
    const std::vector<std::string>& words, const std::vector<unsigned int>& forbiddenPerPosition,
    unsigned int period) {
    std::vector<std::vector<std::vector<Node>>> forbiddenNodesList;

    for (int i = 0; i < period; ++i) {
        unsigned int n = forbiddenPerPosition[i];
        if (n > words.size()) {
            printErrorAndExit("forbidden_per_position value exceeds total combinations.");
        }

        std::vector<Node> forbiddenNodes;
        for (const auto& w : words) {
            forbiddenNodes.emplace_back(w, i);
        }

        forbiddenNodesList.push_back(combine(forbiddenNodes, n, false));
    }

    return forbiddenNodesList;
}

// DFSを用いて禁止ノードを組み合わせる関数
std::vector<std::vector<Node>> combineForbiddenNodes(
    const std::vector<std::vector<std::vector<Node>>>& forbiddenNodesList) {
    std::vector<std::vector<Node>> combinedNodes;
    std::function<void(int, std::vector<Node>)> dfs = [&](int depth, std::vector<Node> current) {
        if (depth == forbiddenNodesList.size()) {
            combinedNodes.push_back(current);
            return;
        }
        for (const auto& nodes : forbiddenNodesList[depth]) {
            auto next = current;
            next.insert(next.end(), nodes.begin(), nodes.end());
            dfs(depth + 1, next);
        }
    };
    dfs(0, {});
    return combinedNodes;
}

// 単語を指定長さまで拡張する
void extendWords(std::vector<std::pair<std::string, unsigned int>>& words, unsigned int targetLen,
                 const std::string& alphabet) {
    std::vector<std::pair<std::string, unsigned int>> extended;

    for (const auto& word : words) {
        if (word.first.size() == targetLen) {
            extended.push_back(word);
        } else {
            for (char c : alphabet) {
                extended.emplace_back(word.first + c, word.second);
            }
        }
    }

    words = std::move(extended);
}

// 禁止語リスト内の単語長を揃える
void formatForDeBruijn(Config& config) {
    auto& words = *config.forbidden_words;
    std::string alphabet = ALPHABET.substr(0, config.alphabet_size);

    // 最大長を取得
    auto getMaxLength = [](const auto& ws) {
        return std::max_element(
                   ws.begin(), ws.end(),
                   [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); })
            ->first.size();
    };

    // 最短長を取得
    auto getMinLength = [](const auto& ws) {
        return std::min_element(
                   ws.begin(), ws.end(),
                   [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); })
            ->first.size();
    };

    config.forbidden_word_length = getMaxLength(words);

    // 長さを揃える
    while (config.forbidden_word_length != getMinLength(words)) {
        extendWords(words, *config.forbidden_word_length, alphabet);
    }
}

// 禁止ノードの生成ロジックを分離
std::vector<std::vector<Node>> generateForbiddenNodes(const Config& config) {
    if (config.mode == "custom") {
        if (config.algorithm == "DeBruijn") {
            formatForDeBruijn(const_cast<Config&>(config));
        }

        // Customモードの禁止ノード生成
        std::vector<Node> forbiddenNodes;
        for (const auto& forbidden : config.forbidden_words.value()) {
            forbiddenNodes.emplace_back(forbidden.first, forbidden.second);
        }
        if (forbiddenNodes.empty()) {
            printErrorAndExit("forbidden_words is empty.");
        }
        return {forbiddenNodes};
    } else if (config.mode == "all-patterns") {
        auto words = combine(ALPHABET.substr(0, config.alphabet_size),
                             config.forbidden_word_length.value(), true);
        auto forbiddenNodesList =
            generateForbiddenNodesList(words, config.forbidden_per_position.value(), config.period);

        return combineForbiddenNodes(forbiddenNodesList);
    } else {
        printErrorAndExit("Unknown mode '" + config.mode + "'.");
    }
    return {};
}

// JSON設定からグラフを生成する関数
void generateGraphFromJson(const std::string& configPath) {
    Config config;
    if (!loadConfig(configPath, config)) {
        printErrorAndExit("Failed to load config.");
    }

    auto forbiddenNodes = generateForbiddenNodes(config);

    std::string baseDirectory = path::genDirPath(config);

    std::unique_ptr<GraphGenerator> generator = createGraphGenerator(config);

    try {
        for (const auto& forbiddenCombinations : forbiddenNodes) {
            Graph graph = generator->generate(forbiddenCombinations);

            if (config.sink_less) {
                graph = cleanGraph(graph);
                if (config.minimize) {
                    graph = Moore::apply(graph);
                }
            }

            for (const auto& format : config.output.formats) {
                if (format == "edges") {
                    saveEdges(baseDirectory, forbiddenCombinations, graph);
                } else if (format == "matrix") {
                    saveAdjacencyMatrix(baseDirectory, forbiddenCombinations, graph);
                }
            }
        }
    } catch (const std::exception& e) {
        printErrorAndExit(e.what());
    }
}

// ファイルパスから拡張子を取得する関数
std::string getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.rfind('.');
    if (dotPos != std::string::npos) {
        return filePath.substr(dotPos);
    }
    return "";
}

int main(int argc, char* argv[]) {
    CLI::App app{"PFT-tools"};

    std::string configPath;
    std::string format;
    bool maxEigenvalue = false;
    unsigned int sequencesLength = 0;

    app.add_option("--config", configPath, "Path to the configuration file or directory")
        ->required();
    app.add_option("--format", format, "Input format: edges, matrix, or directory");
    app.add_flag("--max-eig", maxEigenvalue, "Calculate the maximum eigenvalue");
    app.add_option("--sequences", sequencesLength, "Length of edge label sequences to retrieve");

    CLI11_PARSE(app, argc, argv);

    try {
        std::string extension = getFileExtension(configPath);

        if (extension == ".json") {
            generateGraphFromJson(configPath);
            return 0;
        }

        if (format != "edges" && format != "matrix") {
            printErrorAndExit("Invalid format specified. Use 'edges', 'matrix', or 'directory'.");
        }

        std::vector<std::string> csvFiles;
        if (extension == ".csv") {
            csvFiles.push_back(configPath);
        } else if (extension.empty()) {
            csvFiles = path::getCsvFiles(configPath);
            if (csvFiles.empty()) {
                printErrorAndExit("No CSV files found in the specified directory.");
            }
        } else {
            printErrorAndExit("Unsupported file extension: " + extension);
        }

        for (const auto& csvFile : csvFiles) {
            Graph graph;
            if (format == "edges") {
                if (!loadEdges(csvFile, graph)) {
                    continue;
                }
            } else if (format == "matrix") {
                if (!loadAdjacencyMatrix(csvFile, graph)) {
                    continue;
                }
            }

            if (sequencesLength > 0 && format == "edges") {
                const auto& sequences = graph.getEdgeLabelSequences(sequencesLength);

                std::string directory = path::getDirectory(csvFile, 2);
                std::string fileName = path::getFileName(csvFile);
                std::string filePath = directory + "/sequences/" + fileName;
                saveSequences(filePath, sequences);
            }

            if (maxEigenvalue) {
                std::cout << "Max Eigenvalue: " << calculateMaxEigenvalue(graph) << std::endl;
            }

            std::string directory = path::getDirectory(csvFile, 3);
            std::string dotFilePath = directory + "/graphviz/graph.dot";
            std::string texFilePath = directory + "/graphviz/graph.tex";
            std::string pdfFilePath = directory + "/graphviz/graph.pdf";

            graphviz::saveDotFile(dotFilePath, graph);
            graphviz::cvtDot2Tex(dotFilePath, texFilePath);
            graphviz::cvtTex2PDF(texFilePath, pdfFilePath);
        }
    } catch (const std::exception& e) {
        printErrorAndExit(e.what());
    }

    return 0;
}
