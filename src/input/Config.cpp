#include "Config.hpp"

void from_json(const nlohmann::json& j, OutputConfig& o) {
    j.at("formats").get_to(o.formats);
    j.at("directory").get_to(o.directory);
}

void from_json(const nlohmann::json& j, Config& c) {
    j.at("mode").get_to(c.mode);
    j.at("algorithm").get_to(c.algorithm);
    j.at("sink_less").get_to(c.sink_less);
    j.at("minimize").get_to(c.minimize);
    j.at("alphabet_size").get_to(c.alphabet_size);
    j.at("period").get_to(c.period);

    if (j.contains("forbidden_word_length")) {
        c.forbidden_word_length = j.at("forbidden_word_length").get<unsigned int>();
    }
    if (j.contains("forbidden_words")) {
        c.forbidden_words = j.at("forbidden_words").get<std::vector<std::pair<std::string, unsigned int>>>();
    }
    if (j.contains("forbidden_per_position")) {
        c.forbidden_per_position = j.at("forbidden_per_position").get<std::vector<unsigned int>>();
    }
    j.at("output").get_to(c.output);
}

void Config::validate() const {
    if (mode == "custom") {
        if (!forbidden_words.has_value()) {
            throw std::invalid_argument("In 'custom' mode, 'forbidden_words' is required.");
        }
    } else if (mode == "all-patterns") {
        if (!forbidden_word_length.has_value()) {
            throw std::invalid_argument("In 'all-patterns' mode, 'forbidden_word_length' is required.");
        }
        if (!forbidden_per_position.has_value()) {
            throw std::invalid_argument("In 'all-patterns' mode, 'forbidden_per_position' is required.");
        }
    } else {
        throw std::invalid_argument("Invalid mode: " + mode);
    }
}
