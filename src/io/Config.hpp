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

struct OutputConfig {
    std::vector<std::string> formats;
    std::string directory;
};

struct Word {
    std::string label;
    unsigned int phase;

    Word(const std::string& label, unsigned int phase) : label(label), phase(phase) {}

    size_t hashCode() const {
        return std::hash<std::string>()(label) ^ std::hash<unsigned int>()(phase);
    }
};

constexpr size_t WORD_LABEL_PHASE_PAIR_SIZE = 2;

struct Config {
    std::string mode;
    std::string algorithm;
    bool sink_less;
    bool minimize;
    unsigned int alphabet_size;
    unsigned int period;
    std::optional<unsigned int> forbidden_word_length;
    std::optional<std::vector<Word>> forbidden_words;
    std::optional<std::vector<unsigned int>> forbidden_per_position;
    OutputConfig output;

    void validate() const;
    void formatForDeBruijn();

   private:
    void extendWords(std::vector<Word>& words, unsigned int targetLen,
                     const std::string& alphabet) const;
};

void from_json(const json& j, OutputConfig& o);
void from_json(const json& j, Config& c);

}  // namespace io::type

namespace std {
template <>
struct hash<io::type::Word> {
    size_t operator()(const io::type::Word& word) const { return word.hashCode(); }
};
}  // namespace std
