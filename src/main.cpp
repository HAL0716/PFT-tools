#include <CLI/CLI.hpp>
#include <iostream>
#include <map>     // std::map
#include <memory>  // std::unique_ptr

#include "algorithm/Beal.hpp"
#include "algorithm/DeBruijn.hpp"
#include "algorithm/Moore.hpp"
#include "analysis/eigenvalues.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "core/constants.hpp"
#include "input/input.hpp"
#include "input/loader.hpp"
#include "output/output.hpp"
#include "utils/CombinationUtils.hpp"
#include "utils/GraphUtils.hpp"

using json = nlohmann::json;

// エラーメッセージを一元管理
void printErrorAndExit(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
}

// 動的にGraphGeneratorの派生クラスを生成するファクトリ関数
std::unique_ptr<GraphGenerator> createGraphGenerator(const std::string& algorithm,
                                                     unsigned int alphabetSize, unsigned int period,
                                                     unsigned int wordLength) {
    static const std::map<std::string, std::function<std::unique_ptr<GraphGenerator>()>>
        generatorMap = {
            {"Beal", [=]() { return std::make_unique<Beal>(alphabetSize, period, wordLength); }},
            {"DeBruijn",
             [=]() { return std::make_unique<DeBruijn>(alphabetSize, period, wordLength); }}};

    auto it = generatorMap.find(algorithm);
    if (it != generatorMap.end()) {
        return it->second();
    } else {
        throw std::invalid_argument("Unknown algorithm: " + algorithm);
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

// 禁止ノードの生成ロジックを分離
std::vector<std::vector<Node>> generateForbiddenNodes(const json& config, const std::string& mode,
                                                      unsigned int alphabetSize,
                                                      unsigned int wordLength,
                                                      unsigned int period) {
    if (mode == "custom") {
        // Customモードの禁止ノード生成
        std::vector<Node> forbiddenNodes;
        for (const auto& forbidden : config["forbidden_words"]) {
            if (forbidden.size() != 2) {
                printErrorAndExit("Each entry in forbidden_list must have 'word' and 'position'.");
            }
            forbiddenNodes.emplace_back(forbidden[0], forbidden[1]);
        }
        if (forbiddenNodes.empty()) {
            printErrorAndExit("forbidden_words is empty.");
        }
        return {forbiddenNodes};
    } else if (mode == "all-patterns") {
        // all-patternsモードの禁止ノード生成
        if (!config.contains("forbidden_per_position")) {
            printErrorAndExit("forbidden_per_position is missing.");
        }

        auto forbiddenPerPosition =
            config.value("forbidden_per_position", std::vector<unsigned int>());
        if (forbiddenPerPosition.size() != period) {
            printErrorAndExit("forbidden_per_position size must match the period.");
        }

        auto words = combine(ALPHABET.substr(0, alphabetSize), wordLength, true);
        auto forbiddenNodesList = generateForbiddenNodesList(words, forbiddenPerPosition, period);

        return combineForbiddenNodes(forbiddenNodesList);
    } else {
        printErrorAndExit("Unknown mode '" + mode + "'.");
    }
    return {};
}

// JSON設定からグラフを生成する関数
void generateGraphFromJson(const std::string& configPath) {
    std::cout << "Generating graph from JSON config: " << configPath << std::endl;

    json config;
    if (!loadConfig(configPath, config)) {
        printErrorAndExit("Failed to load config.");
    }

    std::string mode = config.value("mode", "");
    unsigned int alphabetSize = config.value("alphabet_size", 0);
    unsigned int wordLength = config.value("forbidden_word_length", 0);
    unsigned int period = config.value("period", 0);
    std::string algorithm = config.value("algorithm", "");
    bool sinkLess = config.value("sink_less", false);
    bool minimize = config.value("minimize", false);

    auto forbiddenNodes = generateForbiddenNodes(config, mode, alphabetSize, wordLength, period);

    std::string baseDirectory = genOutputPath(config);

    try {
        // 動的にグラフ生成クラスを作成
        std::unique_ptr<GraphGenerator> generator =
            createGraphGenerator(algorithm, alphabetSize, period, wordLength);

        for (const auto& forbiddenCombinations : forbiddenNodes) {
            Graph graph = generator->generate(forbiddenCombinations);

            if (sinkLess) {
                graph = cleanGraph(graph);
                if (minimize) {
                    graph = Moore::apply(graph);
                }
            }

            std::cout << "Generated graph with " << graph.getNodes().size() << " nodes and "
                      << graph.getEdges().size() << " edges." << std::endl;

            for (const auto& format : config["output"]["formats"]) {
                if (format == "edges") {
                    saveEdges(baseDirectory, forbiddenCombinations, graph.getEdges());
                } else if (format == "matrix") {
                    saveAdjacencyMatrix(baseDirectory, forbiddenCombinations, graph.getNodes(),
                                        graph.getEdges());
                }
            }
        }
    } catch (const std::exception& e) {
        printErrorAndExit(e.what());
    }

    std::cout << "Graph generation completed." << std::endl;
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

    app.add_option("--config", configPath, "Path to the configuration file or directory")
        ->required();
    app.add_option("--format", format, "Input format: edges, matrix, or directory");
    app.add_flag("--max-eig", maxEigenvalue, "Calculate the maximum eigenvalue");

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
            csvFiles = loader::getCsvFiles(configPath);
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

            if (maxEigenvalue) {
                std::cout << "Max Eigenvalue: " << calculateMaxEigenvalue(graph) << std::endl;
            }
        }
    } catch (const std::exception& e) {
        printErrorAndExit(e.what());
    }

    return 0;
}
