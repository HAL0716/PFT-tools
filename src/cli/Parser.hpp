#pragma once

#include <CLI/CLI.hpp>
#include <string>

namespace CLI {

class Parser {
   public:
    struct ParsedOptions {
        std::string inputPath;
        std::string format;
        bool isMatrix = false;
        bool pdf = false;
        bool png = false;
        bool maxEig = false;
        unsigned int seqLength = 0;
    };

    Parser();
    ParsedOptions parse(int argc, char* argv[]);
    void validate();

   private:
    CLI::App app{"PFT-tools"};
    ParsedOptions options;
};

}  // namespace CLI
