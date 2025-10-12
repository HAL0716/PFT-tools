#include <iostream>
#include "algorithm/DeBruijn.hpp"
#include "core/Node.hpp"
#include "io/input.hpp"
#include "io/output.hpp"

using json = nlohmann::json;

namespace {

void generateFromConfig(const std::string& configPath) {
    json config;
    if (!loadConfig(configPath, config)) return;

    unsigned int alphabetSize = config.value("alphabet_size", 0);
    unsigned int wordLength = config.value("forbidden_word_length", 0);
    unsigned int period = config.value("period", 0);


    if (alphabetSize < 2 || wordLength < 1 || period < 1) {
        std::cerr << "Error: Invalid config values." << std::endl;
        return;
    }

    DeBruijn generator(alphabetSize, wordLength, period);
    auto forbiddenNodes = parseForbiddenNodes(config);
    Graph graph = generator.generate(forbiddenNodes);

    std::cout << "Generated De Bruijn graph with " << graph.getNodes().size() << " nodes and " << graph.getEdges().size() << " edges." << std::endl;
    for (const auto& edge : graph.getEdges()) {
        std::cout << edge << std::endl;
    }
}

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " generate --config <config_path>" << std::endl;
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc == 4 && argv[1] == std::string("generate") && argv[2] == std::string("--config")) {
        generateFromConfig(argv[3]);
        return 0;
    }
    printUsage(argv[0]);
    return 1;
}
