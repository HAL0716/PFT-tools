#include <iostream>
#include <map>      // std::map
#include <memory>   // std::unique_ptr
#include <numeric>  // std::transform_reduce
#include <string>

#include "algorithm/Beal.hpp"
#include "algorithm/DeBruijn.hpp"
#include "algorithm/Moore.hpp"
#include "analysis/eigenvalues.hpp"
#include "cli/Parser.hpp"
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
    CLI::Parser cliParser;
    auto options = cliParser.parse(argc, argv);

    auto extension = path::utils::extractPath(options.inputPath, 0, false, false, true);

    try {
        if (extension == ".json") {
            io::type::Config config;
            if (!io::input::readConfigJson(options.inputPath, config)) {
                io::utils::printErrorAndExit("Failed to load config.");
            }

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

            auto forbiddenNodesList = io::input::genNodesFromConfig(config);
            for (const auto& forbiddenNodes : forbiddenNodesList) {
                Graph graph = generator->generate(forbiddenNodes);

                if (config.generation.opt_mode == "sink_less") {
                    graph = cleanGraph(graph);
                } else if (config.generation.opt_mode == "minimize") {
                    graph = cleanGraph(graph);
                    graph = Moore::apply(graph);
                }

                path::Generator pathGenerator(config, forbiddenNodes);

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
            cliParser.validate();

            std::vector<std::string> csvFiles =
                (extension == ".csv") ? std::vector<std::string>{options.inputPath}
                                      : path::utils::getFiles(options.inputPath, ".csv");

            if (csvFiles.empty()) {
                io::utils::printErrorAndExit("No CSV files found in the specified directory.");
            }

            for (const auto& csvFile : csvFiles) {
                Graph graph;
                bool success = (options.format == "edges")
                                   ? io::input::readEdgesCSV(csvFile, graph)
                                   : io::input::readMatrixCSV(csvFile, graph);
                if (!success) {
                    continue;
                }

                std::string directory = path::utils::extractPath(csvFile, 2, true, false, false);
                std::string fileName = path::utils::extractPath(csvFile, 0, false, true, false);

                if (options.format != "matrix" && options.isMatrix) {
                    std::string filePath = directory + "/matrix/" + fileName + ".csv";
                    io::output::writeMatrixCsv(filePath, graph);
                }

                if (options.pdf) {
                    std::string pdfPath = directory + "/graph/" + fileName + ".pdf";
                    io::output::writePdf(pdfPath, graph);
                }

                if (options.maxEig) {
                    std::cout << fileName << ": Max Eigenvalue = " << calculateMaxEigenvalue(graph)
                              << std::endl;
                }

                if (options.seqLength > 0) {
                    std::string filePath = directory + "/sequences/" + fileName + ".csv";
                    io::output::writeSeqCsv(filePath, graph, options.seqLength);
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
