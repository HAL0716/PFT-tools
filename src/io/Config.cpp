#include "Config.hpp"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace io::type {

void Node::validate() const {
    // TODO: ロジックを追加
}

void ForbiddenConfig::validate() const {
    if (nodes.empty()) {
        if (length == 0) {
            throw std::invalid_argument("ForbiddenConfig length must be greater than 0.");
        }
        if (position.empty()) {
            throw std::invalid_argument("ForbiddenConfig position cannot be empty.");
        }
    } else {
        for (const auto& node : nodes) {
            node.validate();
        }
    }
}

void GenericConfig::validate() const {
    if (mode.empty()) {
        throw std::invalid_argument("Mode cannot be empty.");
    }
    if (algorithm.empty()) {
        throw std::invalid_argument("Algorithm cannot be empty.");
    }
    if (opt_mode.empty()) {
        throw std::invalid_argument("Optimization mode cannot be empty.");
    }
    if (alphabet < 2) {
        throw std::invalid_argument("Alphabet size must be at least 2.");
    }
    if (mode == "custom" && period == 0) {
        throw std::invalid_argument("Period must be greater than 0.");
    }
    forbidden.validate();
}

void OutputConfig::validate() const {
    if (output_dir.empty()) {
        throw std::invalid_argument("Output directory cannot be empty.");
    }
    if (!edge_list && !png_file) {
        throw std::invalid_argument(
            "At least one output format (edge_list or png_file) must be enabled.");
    }
}

void Config::validate() const {
    generation.validate();
    output.validate();
}

void GenericConfig::extendWords(std::vector<Node>& nodes, unsigned int targetLen,
                                const std::string& alphabet) const {
    std::vector<Node> extended;
    size_t reserve_size = 0;
    for (const auto& node : nodes) {
        reserve_size += (node.label.size() == targetLen) ? 1 : alphabet.size();
    }
    extended.reserve(reserve_size);

    for (auto& node : nodes) {
        if (node.label.size() == targetLen) {
            extended.push_back(std::move(node));
        } else {
            for (char c : alphabet) {
                extended.emplace_back(Node{node.label + c, node.phase});
            }
        }
    }

    nodes = std::move(extended);
}

void GenericConfig::formatForDeBruijn() {
    if (forbidden.nodes.empty()) {
        throw std::invalid_argument("forbidden.nodes is not set.");
    }

    std::vector<Node> nodes(forbidden.nodes.begin(), forbidden.nodes.end());

    auto getMaxLength = [](const auto& ws) {
        return std::max_element(
                   ws.begin(), ws.end(),
                   [](const auto& a, const auto& b) { return a.label.size() < b.label.size(); })
            ->label.size();
    };

    auto getMinLength = [](const auto& ws) {
        return std::min_element(
                   ws.begin(), ws.end(),
                   [](const auto& a, const auto& b) { return a.label.size() < b.label.size(); })
            ->label.size();
    };

    forbidden.length = getMaxLength(nodes);

    while (forbidden.length != getMinLength(nodes)) {
        extendWords(nodes, forbidden.length, ALPHABET.substr(0, alphabet));
    }

    forbidden.nodes = std::vector<Node>(nodes.begin(), nodes.end());
}

void from_json(const json& j, OutputConfig& o) {
    j.at("edge_list").get_to(o.edge_list);
    j.at("png_file").get_to(o.png_file);
    j.at("output_dir").get_to(o.output_dir);
}

void from_json(const json& j, GenericConfig& g) {
    j.at("mode").get_to(g.mode);
    j.at("algorithm").get_to(g.algorithm);
    j.at("opt_mode").get_to(g.opt_mode);
    j.at("alphabet").get_to(g.alphabet);

    if (j.contains("forbidden")) {
        const auto& forbidden = j.at("forbidden");
        if (forbidden.contains("nodes")) {
            for (const auto& item : forbidden.at("nodes")) {
                g.forbidden.nodes.emplace_back(item.at("word").get<std::string>(),
                                               item.at("pos").get<unsigned int>());
            }
        }
        if (forbidden.contains("length")) {
            g.forbidden.length = forbidden.at("length").get<unsigned int>();
        }
        if (forbidden.contains("position")) {
            g.forbidden.position = forbidden.at("position").get<std::vector<unsigned int>>();
        }
    }

    g.period = j.contains("period") ? j.at("period").get<unsigned int>() : g.forbidden.position.size();
}

void from_json(const json& j, Config& c) {
    j.at("generation").get_to(c.generation);
    j.at("output").get_to(c.output);
}

}  // namespace io::type
