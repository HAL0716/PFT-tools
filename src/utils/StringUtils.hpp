#pragma once

#include <vector>
#include <string>
#include <functional>

// 組み合わせ生成ヘルパー関数
inline std::vector<std::string> generateCombinations(const std::string& alphabet, unsigned int wordLength) {
    std::vector<std::string> combinations;

    // 再帰的ラムダ式による組み合わせ生成
    std::function<void(std::string)> generate = [&](std::string current) {
        if (current.size() == wordLength) {
            combinations.push_back(current);
            return;
        }
        for (char c : alphabet) {
            generate(current + c);
        }
    };

    generate("");
    return combinations;
}
