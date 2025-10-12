#include "gtest/gtest.h"
#include "algorithm/DeBruijn.hpp"
#include "core/Node.hpp"

// DeBruijn クラスのテスト

TEST(DeBruijnTest, GenerateGraphBasic) {
    unsigned int alphabetSize = 3;
    unsigned int wordLength = 2;
    unsigned int period = 2;

    DeBruijn generator(alphabetSize, wordLength, period);

    // 禁止ノードなしでグラフを生成
    std::vector<Node> forbiddenNodes;
    Graph graph = generator.generate(forbiddenNodes);

    // ノード数とエッジ数を確認
    EXPECT_EQ(graph.getNodes().size(), 18); // 3^2 * 2 = 18
    EXPECT_EQ(graph.getEdges().size(), 54); // 18 * 3 = 54
}

TEST(DeBruijnTest, GenerateGraphWithForbiddenNodes) {
    unsigned int alphabetSize = 3;
    unsigned int wordLength = 2;
    unsigned int period = 2;

    DeBruijn generator(alphabetSize, wordLength, period);

    // 禁止ノードを設定
    std::vector<Node> forbiddenNodes = {Node("01", 0), Node("12", 1)};
    Graph graph = generator.generate(forbiddenNodes);

    // 禁止ノードが含まれていないことを確認
    for (const auto& node : graph.getNodes()) {
        EXPECT_TRUE(std::find(forbiddenNodes.begin(), forbiddenNodes.end(), node) == forbiddenNodes.end());
    }

    // エッジの始点と終点が禁止ノードに含まれていないことを確認
    for (const auto& edge : graph.getEdges()) {
        EXPECT_TRUE(std::find(forbiddenNodes.begin(), forbiddenNodes.end(), edge.getSource()) == forbiddenNodes.end());
        EXPECT_TRUE(std::find(forbiddenNodes.begin(), forbiddenNodes.end(), edge.getTarget()) == forbiddenNodes.end());
    }
}
