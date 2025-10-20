#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "core/constants.hpp"
#include "nlohmann/json.hpp"

namespace io::type {

using json = nlohmann::json;

constexpr size_t NODE_DATA_SIZE = 2;

struct Node {
    std::string label;
    unsigned int phase;

    Node(const std::string& label, unsigned int phase) : label(label), phase(phase) {}

    void validate() const;
};

struct ForbiddenConfig {
    std::vector<Node> nodes;
    unsigned int length;
    std::vector<unsigned int> position;

    void validate() const;
};

struct GenericConfig {
    std::string mode;
    std::string algorithm;
    std::string opt_mode;
    unsigned int alphabet;
    unsigned int period;
    ForbiddenConfig forbidden;

    void validate() const;
    void formatForDeBruijn();

   private:
    void extendWords(std::vector<Node>& words, unsigned int targetLen,
                     const std::string& alphabet) const;
};

struct OutputConfig {
    bool edge_list;
    bool png_file;
    std::string output_dir;

    void validate() const;
};

struct Config {
    GenericConfig generation;
    OutputConfig output;

    void validate() const;
};

void from_json(const json& j, Config& c);
void from_json(const json& j, OutputConfig& o);
void from_json(const json& j, GenericConfig& g);

}  // namespace io::type
