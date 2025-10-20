#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>

#include "algorithm/GeneratorFactory.hpp"
#include "algorithm/Moore.hpp"
#include "analysis/eigenvalues.hpp"
#include "cli/Parser.hpp"
#include "core/Graph.hpp"
#include "io/Config.hpp"
#include "io/Input.hpp"
#include "io/Output.hpp"
#include "io/utils.hpp"
#include "path/Generator.hpp"
#include "path/utils.hpp"
#include "utils/GraphUtils.hpp"

template <typename PathGen, typename WriteFunc>
void writeGraph(const std::string& type, const std::string& ext, const PathGen& pathGen,
                WriteFunc writeFunc, Graph& graph) {
    const std::string filePath = pathGen(type, ext);
    writeFunc(filePath, graph);
}

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

            std::unique_ptr<GraphGenerator> generator = GeneratorFactory::create(config);

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

                auto generateFilePath = [&](const std::string& type, const std::string& ext) {
                    return pathGenerator.genFilePath(type, ext);
                };

                if (config.output.edge_list) {
                    writeGraph("edges", "csv", generateFilePath, io::output::writeEdgesCsv, graph);
                }

                if (config.output.png_file) {
                    writeGraph("graph", "png", generateFilePath, io::output::writePng, graph);
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

                auto generateFilePath = [&](const std::string& type, const std::string& ext) {
                    return directory + "/" + type + "/" + fileName + "." + ext;
                };

                if (options.format != "matrix" && options.isMatrix) {
                    writeGraph("matrix", "csv", generateFilePath, io::output::writeMatrixCsv,
                               graph);
                }

                if (options.pdf) {
                    writeGraph("graph", "pdf", generateFilePath, io::output::writePdf, graph);
                }

                auto writeSeqCsvWithLength = [&](const std::string& filePath, const Graph& graph) {
                    io::output::writeSeqCsv(filePath, graph, options.seqLength);
                };

                if (options.seqLength > 0) {
                    writeGraph("sequences", "csv", generateFilePath, writeSeqCsvWithLength, graph);
                }

                if (options.maxEig) {
                    std::cout << fileName << ": Max Eigenvalue = " << calculateMaxEigenvalue(graph)
                              << std::endl;
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
