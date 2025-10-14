#include <iostream>
#include <memory> // std::unique_ptr
#include <map> // std::map
#include "algorithm/Beal.hpp"
#include "algorithm/DeBruijn.hpp"
#include "analysis/eigenvalues.hpp"
#include "core/constants.hpp"
#include "core/Node.hpp"
#include "io/input.hpp"
#include "io/output.hpp"
#include "utils/CombinationUtils.hpp"

using json = nlohmann::json;

// エラーメッセージを一元管理
void printErrorAndExit(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
}

// 動的にGraphGeneratorの派生クラスを生成するファクトリ関数
std::unique_ptr<GraphGenerator> createGraphGenerator(const std::string& algorithm, unsigned int alphabetSize, unsigned int period, unsigned int wordLength) {
    static const std::map<std::string, std::function<std::unique_ptr<GraphGenerator>()>> generatorMap = {
        {"Beal", [=]() { return std::make_unique<Beal>(alphabetSize, period, wordLength); }},
        {"DeBruijn", [=]() { return std::make_unique<DeBruijn>(alphabetSize, period, wordLength); }}
    };

    auto it = generatorMap.find(algorithm);
    if (it != generatorMap.end()) {
        return it->second();
    } else {
        throw std::invalid_argument("Unknown algorithm: " + algorithm);
    }
}

// forbiddenNodesListを生成する関数
std::vector<std::vector<std::vector<Node>>> generateForbiddenNodesList(const std::vector<std::string>& words, const std::vector<unsigned int>& forbiddenPerPosition, unsigned int period) {
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
std::vector<std::vector<Node>> combineForbiddenNodes(const std::vector<std::vector<std::vector<Node>>>& forbiddenNodesList) {
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
std::vector<std::vector<Node>> generateForbiddenNodes(const json& config, const std::string& mode, unsigned int alphabetSize, unsigned int wordLength, unsigned int period) {
    if (mode == "custom") {
        // Customモードの禁止ノード生成
        std::vector<Node> forbiddenNodes;
        if (!config.contains("forbidden_list")) {
            printErrorAndExit("forbidden_list is missing.");
        }

        for (const auto& forbidden : config["forbidden_list"]) {
            if (forbidden.contains("word") && forbidden.contains("position")) {
                forbiddenNodes.emplace_back(forbidden["word"], forbidden["position"]);
            }
        }
        return {forbiddenNodes};
    } else if (mode == "all-patterns") {
        // all-patternsモードの禁止ノード生成
        if (!config.contains("forbidden_per_position")) {
            printErrorAndExit("forbidden_per_position is missing.");
        }

        auto forbiddenPerPosition = config.value("forbidden_per_position", std::vector<unsigned int>());
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

int main(int argc, char* argv[]) {
    if (argc != 4 || std::string(argv[1]) != "generate" || std::string(argv[2]) != "--config") {
        std::cerr << "Usage: " << argv[0] << " generate --config <config_path>" << std::endl;
        return 1;
    }

    json config;
    if (!loadConfig(argv[3], config)) {
        printErrorAndExit("Failed to load config.");
    }

    std::string mode = config.value("mode", "");
    unsigned int alphabetSize = config.value("alphabet_size", 0);
    unsigned int wordLength = config.value("forbidden_word_length", 0);
    unsigned int period = config.value("period", 0);
    std::string algorithm = config.value("algorithm", "");

    auto forbiddenNodes = generateForbiddenNodes(config, mode, alphabetSize, wordLength, period);

    std::string baseDirectory = genOutputPath(config);

    try {
        // 動的にグラフ生成クラスを作成
        std::unique_ptr<GraphGenerator> generator = createGraphGenerator(algorithm, alphabetSize, period, wordLength);

        for (const auto& forbiddenCombinations : forbiddenNodes) {
            Graph graph = generator->generate(forbiddenCombinations);

            saveEdges(baseDirectory, forbiddenCombinations, graph.getEdges());
            saveAdjacencyMatrix(baseDirectory, forbiddenCombinations, graph.getNodes(), graph.getEdges());

            // 固有値計算と表示
            double maxEigenvalue = calculateMaxEigenvalue(graph);
            std::cout << "Max Eigenvalue: " << maxEigenvalue << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
