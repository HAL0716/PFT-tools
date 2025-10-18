#include "ForbiddenNodes.hpp"

#include <functional>
#include <stdexcept>
#include <utility>

#include "../utils/utils.hpp"
#include "utils/CombinationUtils.hpp"

namespace detail {

std::vector<std::vector<std::vector<Node>>> genForbiddenList(const Config& config) {
    std::vector<std::vector<std::vector<Node>>> forbiddenNodesList;
    const auto& words = combine(ALPHABET.substr(0, config.alphabet_size),
                                config.forbidden_word_length.value(), true);
    const auto& perPosition = config.forbidden_per_position.value();
    unsigned int period = std::min(config.period, static_cast<unsigned int>(perPosition.size()));

    for (unsigned int p = 0; p < period; ++p) {
        unsigned int n = perPosition[p];
        if (n > words.size()) {
            io::utils::printErrorAndExit(
                "forbidden_per_position value exceeds total combinations.");
        }

        std::vector<Node> forbiddenNodes;
        for (const auto& w : words) {
            forbiddenNodes.emplace_back(w, p);
        }

        // 組み合わせを生成
        forbiddenNodesList.emplace_back(n == 0 ? std::vector<std::vector<Node>>()
                                        : n == words.size()
                                            ? std::vector<std::vector<Node>>{forbiddenNodes}
                                            : combine(forbiddenNodes, n, false));
    }

    return forbiddenNodesList;
}

std::vector<std::vector<Node>> combineNodes(
    const std::vector<std::vector<std::vector<Node>>>& forbiddenList) {
    std::vector<std::vector<Node>> combinedNodes;
    std::function<void(unsigned int, std::vector<Node>)> dfs = [&](unsigned int depth,
                                                                   std::vector<Node> current) {
        if (depth == forbiddenList.size()) {
            combinedNodes.push_back(std::move(current));
            return;
        }
        for (const auto& nodes : forbiddenList[depth]) {
            auto next = current;
            next.insert(next.end(), nodes.begin(), nodes.end());
            dfs(depth + 1, std::move(next));
        }
    };
    dfs(0, {});
    return combinedNodes;
}

}  // namespace detail

std::vector<std::vector<Node>> genNodesFromConfig(const Config& config) {
    if (config.mode == "custom") {
        std::vector<Node> forbiddenNodes;
        for (const auto& forbidden : config.forbidden_words.value()) {
            forbiddenNodes.emplace_back(forbidden.first, forbidden.second);
        }
        if (forbiddenNodes.empty()) {
            io::utils::printErrorAndExit("forbidden_words is empty.");
        }
        return {std::move(forbiddenNodes)};
    } else if (config.mode == "all-patterns") {
        auto forbiddenNodesList = detail::genForbiddenList(config);
        return detail::combineNodes(forbiddenNodesList);
    } else {
        io::utils::printErrorAndExit("Unknown mode '" + config.mode + "'.");
    }
    return {};
}
