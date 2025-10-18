#include <CLI/CLI.hpp>
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
#include "io/formats/csv/csv.hpp"
#include "io/formats/json/Config.hpp"
#include "io/input/Input.hpp"
#include "io/output/Output.hpp"
#include "io/utils/ForbiddenNodes.hpp"
#include "io/utils/utils.hpp"
#include "io/visualization/Graphviz.hpp"
#include "path/PathUtils.hpp"
#include "utils/CombinationUtils.hpp"
#include "utils/GraphUtils.hpp"

using json = nlohmann::json;

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

// JSON設定からグラフを生成する関数
void generateGraphFromJson(const std::string& configPath) {
    Config config;
    if (!loadConfig(configPath, config)) {
        io::utils::printErrorAndExit("Failed to load config.");
    }

    auto forbiddenNodes = genNodesFromConfig(config);

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
                } else if (format == "dot") {
                    if (!graphviz::saveDot(baseDirectory, forbiddenCombinations, graph)) {
                        continue;
                    }
                } else if (format == "png") {
                    if (!graphviz::saveDot(baseDirectory, forbiddenCombinations, graph)) {
                        continue;
                    }
                    if (!graphviz::cvtDot2TeX(baseDirectory, forbiddenCombinations)) {
                        continue;
                    }
                    if (!graphviz::cvtTex2PDF(baseDirectory, forbiddenCombinations)) {
                        continue;
                    }
                    if (!graphviz::cvtPDF2PNG(baseDirectory, forbiddenCombinations)) {
                        continue;
                    }
                } else {
                    io::utils::printErrorAndExit("Unknown output format: " + format);
                }
            }
        }
    } catch (const std::exception& e) {
        io::utils::printErrorAndExit(e.what());
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
            io::utils::printErrorAndExit(
                "Invalid format specified. Use 'edges', 'matrix', or 'directory'.");
        }

        std::vector<std::string> csvFiles;
        if (extension == ".csv") {
            csvFiles.push_back(configPath);
        } else if (extension.empty()) {
            csvFiles = path::getCsvFiles(configPath);
            if (csvFiles.empty()) {
                io::utils::printErrorAndExit("No CSV files found in the specified directory.");
            }
        } else {
            io::utils::printErrorAndExit("Unsupported file extension: " + extension);
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
        }
    } catch (const std::exception& e) {
        io::utils::printErrorAndExit(e.what());
    }

    return 0;
}
