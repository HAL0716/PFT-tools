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

void handleInputJson(const CLI::Parser::ParsedOptions& options) {
    io::utils::logMessage("Processing JSON input: " + options.inputPath);

    io::type::Config config;
    if (!io::input::readConfigJson(options.inputPath, config)) {
        io::utils::printErrorAndExit("[ERROR] Failed to load config from: " + options.inputPath);
    }

    std::unique_ptr<GraphGenerator> generator = GeneratorFactory::create(config);

    auto forbiddenNodesList = io::input::genNodesFromConfig(config);
    for (const auto& forbiddenNodes : forbiddenNodesList) {
        Graph graph = generator->generate(forbiddenNodes);

        if (config.generation.opt_mode == "sink_less") {
            io::utils::logMessage("Applying sink-less optimization.");
            graph = cleanGraph(graph);
        } else if (config.generation.opt_mode == "minimize") {
            io::utils::logMessage("Applying minimize optimization.");
            graph = cleanGraph(graph);
            graph = Moore::apply(graph);
        }

        path::Generator pathGenerator(config, forbiddenNodes);

        auto generateFilePath = [&](const std::string& type, const std::string& ext) {
            return pathGenerator.genFilePath(type, ext);
        };

        if (config.output.edge_list) {
            if (io::output::writeGraph("edges", "csv", generateFilePath, io::output::writeEdgesCsv, graph)) {
                io::utils::logMessage("Writing edge list to CSV.");
            }
        }

        if (config.output.png_file) {
            if (io::output::writeGraph("graph", "png", generateFilePath, io::output::writePng, graph)) {
                io::utils::logMessage("Writing graph to PNG.");
            }
        }
    }
}

void handleInputCSV(const CLI::Parser::ParsedOptions& options, const std::string& extension) {
    io::utils::logMessage("Processing CSV input: " + options.inputPath);

    std::vector<std::string> csvFiles = (extension == ".csv")
                                            ? std::vector<std::string>{options.inputPath}
                                            : path::utils::getFiles(options.inputPath, ".csv");

    if (csvFiles.empty()) {
        io::utils::printErrorAndExit("[ERROR] No CSV files found in the specified directory: " +
                                     options.inputPath);
    }

    for (const auto& csvFile : csvFiles) {
        io::utils::logMessage("Reading CSV file: " + csvFile);

        Graph graph;
        bool success = (options.format == "edges") ? io::input::readEdgesCSV(csvFile, graph)
                                                   : io::input::readMatrixCSV(csvFile, graph);
        if (!success) {
            io::utils::logMessage("[WARNING] Failed to read CSV file: " + csvFile);
            continue;
        }

        std::string directory = path::utils::extractPath(csvFile, 2, true, false, false);
        std::string fileName = path::utils::extractPath(csvFile, 0, false, true, false);

        auto generateFilePath = [&](const std::string& type, const std::string& ext) {
            return directory + "/" + type + "/" + fileName + "." + ext;
        };

        if (options.format != "matrix" && options.isMatrix) {
            io::utils::logMessage("Writing adjacency matrix to CSV.");
            io::output::writeGraph("matrix", "csv", generateFilePath, io::output::writeMatrixCsv, graph);
        }

        if (options.pdf) {
            io::utils::logMessage("Writing graph to PDF.");
            io::output::writeGraph("graph", "pdf", generateFilePath, io::output::writePdf, graph);
        }

        auto writeSeqCsvWithLength = [&](const std::string& filePath, const Graph& graph) {
            return io::output::writeSeqCsv(filePath, graph, options.seqLength);
        };

        if (options.seqLength > 0) {
            if (io::output::writeGraph("sequences", "csv", generateFilePath, writeSeqCsvWithLength, graph)) {
                io::utils::logMessage("Writing sequences to CSV.");
            }
        }

        if (options.maxEig) {
            double maxEig = calculateMaxEigenvalue(graph);
            io::utils::logMessage(fileName + ": Max Eigenvalue = " + std::to_string(maxEig));
        }
    }
}

int main(int argc, char* argv[]) {
    CLI::Parser cliParser;
    auto options = cliParser.parse(argc, argv);

    auto extension = path::utils::extractPath(options.inputPath, 0, false, false, true);

    try {
        if (extension == ".json") {
            handleInputJson(options);
        } else if (extension == ".csv" || extension.empty()) {
            cliParser.validate();
            handleInputCSV(options, extension);
        } else {
            io::utils::printErrorAndExit("[ERROR] Unsupported file extension: " + extension);
        }
    } catch (const std::exception& e) {
        io::utils::printErrorAndExit("[ERROR] " + std::string(e.what()));
    }

    return 0;
}
