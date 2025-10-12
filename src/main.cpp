#include <iostream>
#include "algorithm/Beal.hpp"
#include "algorithm/DeBruijn.hpp"
#include "core/Node.hpp"
#include "io/input.hpp"
#include "io/output.hpp"

using json = nlohmann::json;

void generateGraph(const json& config) {
    unsigned int alphabetSize = config.value("alphabet_size", 0);
    unsigned int wordLength = config.value("forbidden_word_length", 0);
    unsigned int period = config.value("period", 0);
    std::string algorithm = config.value("algorithm", "");

    if (alphabetSize < 2 || wordLength < 1 || period < 1) {
        std::cerr << "Error: Invalid config values." << std::endl;
        return;
    }

    auto forbiddenNodes = parseForbiddenNodes(config);
    Graph graph;

    if (algorithm == "Beal") {
        Beal generator(alphabetSize, period);
        graph = generator.generate(forbiddenNodes);
    } else if (algorithm == "DeBruijn") {
        DeBruijn generator(alphabetSize, wordLength, period);
        graph = generator.generate(forbiddenNodes);
    } else {
        std::cerr << "Error: Unknown algorithm '" << algorithm << "'." << std::endl;
        return;
    }

    for (const auto& edge : graph.getEdges()) {
        std::cout << edge << std::endl;
    }

    std::cout << "Total: " << graph.getEdges().size() << " edges." << std::endl;
}

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " generate --config <config_path>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4 || std::string(argv[1]) != "generate" || std::string(argv[2]) != "--config") {
        printUsage(argv[0]);
        return 1;
    }

    json config;
    if (!loadConfig(argv[3], config)) {
        std::cerr << "Error: Failed to load config." << std::endl;
        return 1;
    }

    generateGraph(config);

    return 0;
}
