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

int main(int argc, char* argv[]) {
    CLI::App app{"PFT-tools"};

    std::string inputPath;
    std::string format;
    bool isMatrix = false;
    bool pdf = false;
    bool maxEig = false;
    unsigned int seqLength = 0;

    app.add_option("--input", inputPath, "Input file or directory path (JSON or CSV)")->required();
    app.add_option("--format", format, "Input format: edges, matrix, or directory");
    app.add_flag("--matrix", isMatrix, "Input CSV files are adjacency matrices");
    app.add_flag("--pdf", pdf, "Generate PDF files");
    app.add_flag("--max-eig", maxEig, "Calculate max eigenvalue");
    app.add_option("--sequences", seqLength, "Length of edge label sequences");

    CLI11_PARSE(app, argc, argv);

    auto extension = path::utils::extractPath(inputPath, 0, false, false, true);

    try {
        if (extension == ".json") {
            io::type::Config config;
            if (!io::input::readConfigJson(inputPath, config)) {
                io::utils::printErrorAndExit("Failed to load config.");
            }

            auto forbiddenNodes = io::input::genNodesFromConfig(config);

            static const std::map<std::string,
                                  std::function<std::unique_ptr<GraphGenerator>(const Config&)>>
                generatorMap = {{"Beal",
                                 [](const Config& config) {
                                     return std::make_unique<Beal>(config.generation.alphabet,
                                                                   config.generation.period);
                                 }},
                                {"DeBruijn", [](const Config& config) {
                                     return std::make_unique<DeBruijn>(
                                         config.generation.alphabet, config.generation.period,
                                         config.generation.forbidden.length);
                                 }}};

            auto it = generatorMap.find(config.generation.algorithm);
            if (it == generatorMap.end()) {
                io::utils::printErrorAndExit("Unknown algorithm: " + config.generation.algorithm);
            }

            std::unique_ptr<GraphGenerator> generator = it->second(config);

            for (const auto& forbiddenCombinations : forbiddenNodes) {
                Graph graph = generator->generate(forbiddenCombinations);

                if (config.generation.opt_mode == "sink_less") {
                    graph = cleanGraph(graph);
                } else if (config.generation.opt_mode == "minimize") {
                    graph = cleanGraph(graph);
                    graph = Moore::apply(graph);
                }

                path::Generator pathGenerator(config, forbiddenCombinations);

                if (config.output.edge_list) {
                    const std::string filePath = pathGenerator.genFilePath("edges", "csv");
                    io::output::writeEdgesCsv(filePath, graph);
                }
                if (config.output.png_file) {
                    const std::string filePath = pathGenerator.genFilePath("graph", "png");
                    io::output::writePng(filePath, graph);
                }
            }
        } else if (extension == ".csv" || extension.empty()) {
            if (format != "edges" && format != "matrix") {
                io::utils::printErrorAndExit(
                    "Invalid format specified. Use 'edges' or 'matrix'.");
            } else if (!maxEig && seqLength == 0 && !isMatrix && !pdf) {
                io::utils::printErrorAndExit(
                    "For CSV input, either --max-eig or --sequences must be specified.");
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

                std::string directory = path::utils::extractPath(csvFile, 2, true, false, false);
                std::string fileName = path::utils::extractPath(csvFile, 0, false, true, false);

                if (isMatrix) {
                    std::string filePath = directory + "/matrix/" + fileName + ".csv";
                    io::output::writeMatrixCsv(filePath, graph);
                }

                if (pdf) {
                    std::string pdfPath = directory + "/graph/" + fileName + ".pdf";
                    io::output::writePdf(pdfPath, graph);
                }

                if (maxEig) {
                    std::cout << fileName << ": Max Eigenvalue = " << calculateMaxEigenvalue(graph)
                              << std::endl;
                }

                if (seqLength > 0) {
                    std::string filePath = directory + "/sequences/" + fileName + ".csv";
                    io::output::writeSeqCsv(filePath, graph, seqLength);
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
