#pragma once

#include "../core/Node.hpp"
#include "../core/Graph.hpp"
#include "../core/constants.hpp"
#include <vector>
#include <unordered_map>
#include <functional>

class DeBruijn {
public:
    // コンストラクタ
    DeBruijn(unsigned int alphabetSize, unsigned int period, unsigned int wordLength);

    // グラフ生成
    Graph generate(const std::vector<Node>& forbiddenNodes)  const;

private:
    std::string alphabet; // アルファベット
    unsigned int period;  // 周期
    std::vector<Node> nodes; // ノードリスト
    std::vector<Edge> edges; // エッジリスト

    // ヘルパー関数
    void generateNodes(unsigned int wordLength, unsigned int period); // ノード生成のヘルパー関数
    void generateEdges(); // エッジ生成のヘルパー関数
};
