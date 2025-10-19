#include "path/Generator.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>

namespace path {

std::string getRoot() {
    return std::filesystem::current_path().parent_path().string();
}

std::string buildBaseDir(const Config& cfg, const std::string& root) {
    std::ostringstream path;
    path << root << "/" << cfg.output.directory << "/";

    if (cfg.mode == "all-patterns") {
        path << cfg.alphabet_size << "-" << cfg.period << "-"
             << cfg.forbidden_word_length.value_or(0);

        if (cfg.forbidden_per_position) {
            path << ":";
            const auto& counts = cfg.forbidden_per_position.value();
            for (size_t i = 0; i < counts.size(); ++i) {
                if (i > 0)
                    path << "-";
                path << counts[i];
            }
        }
    } else {
        path << cfg.mode;
    }

    return path.str();
}

std::string buildBaseName(const std::vector<Node>& nodes) {
    std::ostringstream name;
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i > 0)
            name << "-";
        const auto& node = nodes[i];
        name << node.getLabel() << ":" << node.getPhase();
    }
    return name.str();
}

Generator::Generator(const Config& config, const std::vector<Node>& nodes)
    : baseDir(buildBaseDir(config, getRoot())), baseName(buildBaseName(nodes)) {}

std::string Generator::genFilePath(const std::string& subDir, const std::string& ext) {
    std::ostringstream oss;
    oss << baseDir;

    if (!subDir.empty()) {
        oss << "/" << subDir;
    }

    oss << "/" << baseName << "." << ext;
    return oss.str();
}

}  // namespace path
