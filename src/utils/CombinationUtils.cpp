#include "CombinationUtils.hpp"

#include <string>
#include <vector>

// 文字列に対する組み合わせ生成関数
std::vector<std::string> combine(const std::string& chars, unsigned int l, bool withRepetition) {
    std::vector<char> charVec(chars.begin(), chars.end());

    auto combinations = combine(charVec, l, withRepetition);

    std::vector<std::string> result;
    for (const auto& comb : combinations) {
        result.emplace_back(comb.begin(), comb.end());
    }

    return result;
}
