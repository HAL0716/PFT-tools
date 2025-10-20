#include "Parser.hpp"

#include <cstdlib>

#include "io/utils.hpp"

namespace CLI {

Parser::Parser() {
    app.add_option("--input", options.inputPath, "Input file or directory path (JSON or CSV)")
        ->required();
    app.add_option("--format", options.format, "Input format: edges or matrix");
    app.add_flag("--matrix", options.isMatrix, "Generate adjacency matrix CSV files");
    app.add_flag("--pdf", options.pdf, "Generate PDF files");
    app.add_flag("--max-eig", options.maxEig, "Calculate max eigenvalue");
    app.add_option("--sequences", options.seqLength, "Calculate length of edge label sequences");
}

Parser::ParsedOptions Parser::parse(int argc, char* argv[]) {
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        std::exit(app.exit(e));
    }
    return options;
}

void Parser::validate() {
    if (options.format != "edges" && options.format != "matrix") {
        io::utils::printErrorAndExit("Invalid format specified. Use 'edges' or 'matrix'.");
    }

    if (!options.maxEig && options.seqLength == 0 && !options.isMatrix && !options.pdf) {
        io::utils::printErrorAndExit(
            "For CSV input, either --max-eig or --sequences must be specified.");
    }
}

}  // namespace CLI
