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
#include "io/Config.hpp"
#include "io/Input.hpp"
#include "io/Output.hpp"
#include "io/utils.hpp"
#include "path/Generator.hpp"
#include "path/utils.hpp"
#include "utils/CombinationUtils.hpp"
#include "utils/GraphUtils.hpp"

using Config = io::type::Config;
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
    io::type::Config config;
    if (!io::input::readConfigJson(configPath, config)) {
        io::utils::printErrorAndExit("Failed to load config.");
    }

    auto forbiddenNodes = io::input::genNodesFromConfig(config);

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
                path::Generator pathGenerator(config, forbiddenCombinations);

                if (format == "edges") {
                    const std::string filePath = pathGenerator.genFilePath("edges", "csv");
                    io::output::writeEdgesCsv(filePath, graph);
                } else if (format == "matrix") {
                    const std::string filePath = pathGenerator.genFilePath("matrix", "csv");
                    io::output::writeMatrixCsv(filePath, graph);
                } else if (format == "dot") {
                    const std::string filePath = pathGenerator.genFilePath("dot", "dot");
                    io::output::writeDot(filePath, graph);
                } else if (format == "pdf") {
                    const std::string filePath = pathGenerator.genFilePath("pdf", "pdf");
                    io::output::writePdf(filePath, graph);
                } else if (format == "png") {
                    const std::string filePath = pathGenerator.genFilePath("png", "png");
                    io::output::writePng(filePath, graph);
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

    std::string inputPath;
    std::string format;
    bool maxEigenvalue = false;
    unsigned int sequencesLength = 0;

    auto inputOpt =
        app.add_option("--input", inputPath, "Input file or directory path (JSON or CSV)");

    app.add_option("--format", format, "Input format: edges, matrix, or directory");
    app.add_flag("--max-eig", maxEigenvalue, "Calculate the maximum eigenvalue");
    app.add_option("--sequences", sequencesLength, "Length of edge label sequences to retrieve");

    CLI11_PARSE(app, argc, argv);

    try {
        if (inputPath.empty()) {
            io::utils::printErrorAndExit("The --input option must be specified.");
        }

        std::string extension = getFileExtension(inputPath);

        if (extension == ".json") {
            generateGraphFromJson(inputPath);
        } else if (extension == ".csv" || extension.empty()) {
            if (format != "edges" && format != "matrix") {
                io::utils::printErrorAndExit(
                    "Invalid format specified. Use 'edges', 'matrix', or 'directory'.");
            }

            std::vector<std::string> csvFiles;
            if (extension == ".csv") {
                csvFiles.push_back(inputPath);
            } else {
                csvFiles = path::utils::getFiles(inputPath, ".csv");
            }

            if (csvFiles.empty()) {
                io::utils::printErrorAndExit("No CSV files found in the specified directory.");
            }

            for (const auto& csvFile : csvFiles) {
                Graph graph;
                if (format == "edges") {
                    if (!io::input::readEdgesCSV(csvFile, graph)) {
                        continue;
                    }
                } else if (format == "matrix") {
                    if (!io::input::readMatrixCSV(csvFile, graph)) {
                        continue;
                    }
                }

                if (sequencesLength > 0 && format == "edges") {
                    std::string directory =
                        path::utils::extractPath(csvFile, 2, true, false, false);
                    std::string fileName = path::utils::extractPath(csvFile, 0, false, true, true);
                    std::string filePath = directory + "/sequences/" + fileName;
                    io::output::writeSeqCsv(filePath, graph, sequencesLength);
                }

                if (maxEigenvalue) {
                    std::cout << "Max Eigenvalue: " << calculateMaxEigenvalue(graph) << std::endl;
                }
            }
        } else {
            io::utils::printErrorAndExit("Unsupported file extension: " + extension);
        }
    } catch (const std::exception& e) {
        io::utils::printErrorAndExit(e.what());
    }

    return 0;
}
