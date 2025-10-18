#pragma once
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace io::formats::json {

using json = nlohmann::json;

struct OutputConfig {
    std::vector<std::string> formats;
    std::string directory;
};

struct Config {
    std::string mode;
    std::string algorithm;
    bool sink_less;
    bool minimize;
    unsigned int alphabet_size;
    unsigned int period;
    std::optional<unsigned int> forbidden_word_length;
    std::optional<std::vector<std::pair<std::string, unsigned int>>> forbidden_words;
    std::optional<std::vector<unsigned int>> forbidden_per_position;
    OutputConfig output;

    // バリデーション関数
    void validate() const;
};

// JSON とのマッピング
void from_json(const json& j, OutputConfig& o);
void from_json(const json& j, Config& c);

}  // namespace io::formats::json
