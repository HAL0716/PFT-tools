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
    path << root << "/" << cfg.output.output_dir << "/";

    if (cfg.generation.mode == "all-patterns") {
        path << cfg.generation.alphabet << "-" << cfg.generation.period << "-"
             << cfg.generation.forbidden.length;

        if (!cfg.generation.forbidden.position.empty()) {
            path << ":";
            const auto& counts = cfg.generation.forbidden.position;
            for (size_t i = 0; i < counts.size(); ++i) {
                if (i > 0)
                    path << "-";
                path << counts[i];
            }
        }
    } else {
        path << cfg.generation.mode;
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
