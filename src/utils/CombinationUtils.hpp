#pragma once

#include <algorithm>
#include <functional>
#include <vector>

// 復元なしの組み合わせ生成
template <typename T>
std::vector<std::vector<typename T::value_type>> combineNoRep(const T& elems, unsigned int n) {
    std::vector<std::vector<typename T::value_type>> result;
    std::vector<bool> mask(n, true);
    mask.resize(elems.size(), false);

    do {
        std::vector<typename T::value_type> comb;
        for (size_t i = 0; i < elems.size(); ++i) {
            if (mask[i]) {
                comb.push_back(*(std::next(elems.begin(), i)));
            }
        }
        result.push_back(comb);
    } while (std::prev_permutation(mask.begin(), mask.end()));

    return result;
}

// 復元ありの組み合わせ生成
template <typename T>
std::vector<std::vector<typename T::value_type>> combineWithRep(const T& elems, unsigned int n) {
    std::vector<std::vector<typename T::value_type>> result;
    std::vector<typename T::value_type> comb(n);
    std::function<void(unsigned int)> dfs = [&](unsigned int depth) {
        if (depth == n) {
            result.push_back(comb);
            return;
        }
        for (const auto& e : elems) {
            comb[depth] = e;
            dfs(depth + 1);
        }
    };
    dfs(0);

    return result;
}

// 復元あり・なしを切り替え可能なラッパー関数
template <typename T>
std::vector<std::vector<typename T::value_type>> combine(const T& elems, unsigned int n,
                                                         bool withRepetition) {
    if (withRepetition) {
        return combineWithRep(elems, n);
    } else {
        return combineNoRep(elems, n);
    }
}

// Declaration only for the string-based combine function
std::vector<std::string> combine(const std::string& chars, unsigned int l, bool withRepetition);
