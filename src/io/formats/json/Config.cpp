#include "Config.hpp"

#include <stdexcept>  // for std::invalid_argument
#include <string>     // for std::string
#include <utility>    // for std::pair
#include <vector>     // for std::vector

namespace io::formats::json {

void Config::validate() const {
    if (mode == "custom") {
        if (!forbidden_words.has_value()) {
            throw std::invalid_argument("In 'custom' mode, 'forbidden_words' is required.");
        }
    } else if (mode == "all-patterns") {
        if (!forbidden_word_length.has_value()) {
            throw std::invalid_argument(
                "In 'all-patterns' mode, 'forbidden_word_length' is required.");
        }
        if (!forbidden_per_position.has_value()) {
            throw std::invalid_argument(
                "In 'all-patterns' mode, 'forbidden_per_position' is required.");
        }
    } else {
        throw std::invalid_argument("Invalid mode: " + mode);
    }
}

void Config::extendWords(std::vector<std::pair<std::string, unsigned int>>& words,
                         unsigned int targetLen, const std::string& alphabet) const {
    std::vector<std::pair<std::string, unsigned int>> extended;

    for (const auto& word : words) {
        if (word.first.size() == targetLen) {
            extended.push_back(word);
        } else {
            for (char c : alphabet) {
                extended.emplace_back(word.first + c, word.second);
            }
        }
    }

    words = std::move(extended);
}

void Config::formatForDeBruijn() {
    if (!forbidden_words.has_value()) {
        throw std::invalid_argument("forbidden_words is not set.");
    }

    auto& words = *forbidden_words;
    std::string alphabet = ALPHABET.substr(0, alphabet_size);

    // 最大長を取得
    auto getMaxLength = [](const auto& ws) {
        return std::max_element(
                   ws.begin(), ws.end(),
                   [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); })
            ->first.size();
    };

    // 最短長を取得
    auto getMinLength = [](const auto& ws) {
        return std::min_element(
                   ws.begin(), ws.end(),
                   [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); })
            ->first.size();
    };

    forbidden_word_length = getMaxLength(words);

    // 長さを揃える
    while (forbidden_word_length != getMinLength(words)) {
        extendWords(words, *forbidden_word_length, alphabet);
    }
}

void from_json(const json& j, OutputConfig& o) {
    j.at("formats").get_to(o.formats);
    j.at("directory").get_to(o.directory);
}

void from_json(const json& j, Config& c) {
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
        c.forbidden_words =
            j.at("forbidden_words").get<std::vector<std::pair<std::string, unsigned int>>>();
    }
    if (j.contains("forbidden_per_position")) {
        c.forbidden_per_position = j.at("forbidden_per_position").get<std::vector<unsigned int>>();
    }
    j.at("output").get_to(c.output);
}

}  // namespace io::formats::json
