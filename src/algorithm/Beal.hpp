#pragma once

#include "../core/Node.hpp"
#include "../core/Graph.hpp"
#include "../core/constants.hpp"
#include <vector>

class Beal {
public:
    // コンストラクタ
    Beal(unsigned int alphabetSize, unsigned int period);

    // グラフ生成
    Graph generate(const std::vector<Node>& forbiddenNodes)  const;

private:
    std::string alphabet; // アルファベット
    unsigned int period;  // 周期


    // ヘルパー関数
    std::vector<Node> generateNodes(const std::vector<Node>& forbiddenNodes) const; // ノード生成のヘルパー関数
};
