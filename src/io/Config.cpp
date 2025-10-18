#include "Config.hpp"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace io::type {

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

void Config::extendWords(std::vector<Word>& words, unsigned int targetLen,
                         const std::string& alphabet) const {
    std::vector<Word> extended;

    for (const auto& word : words) {
        if (word.label.size() == targetLen) {
            extended.push_back(word);
        } else {
            for (char c : alphabet) {
                extended.emplace_back(Word{word.label + c, word.phase});
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

    forbidden_word_length = getMaxLength(words);

    while (*forbidden_word_length != getMinLength(words)) {
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
        auto isValidWord = [](const json& item) {
            return item.is_array() && item.size() == 2 && item[0].is_string() &&
                   item[1].is_number_unsigned();
        };

        std::vector<Word> processedWords;
        for (const auto& item : j.at("forbidden_words")) {
            if (!isValidWord(item)) {
                throw std::invalid_argument(
                    "Invalid format for forbidden_words. Each item must be [string, unsigned "
                    "int].");
            }
            processedWords.push_back(Word{item[0].get<std::string>(), item[1].get<unsigned int>()});
        }
        c.forbidden_words = std::move(processedWords);
    }
    if (j.contains("forbidden_per_position")) {
        c.forbidden_per_position = j.at("forbidden_per_position").get<std::vector<unsigned int>>();
    }

    j.at("output").get_to(c.output);
}

}  // namespace io::type
