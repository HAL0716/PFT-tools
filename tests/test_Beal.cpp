#include "../src/algorithm/Beal.hpp"
#include "../src/core/Node.hpp"
#include "../src/core/Graph.hpp"
#include <gtest/gtest.h>

TEST(BealTest, GenerateNodes) {
    unsigned int alphabetSize = 3;
    unsigned int period = 2;
    Beal beal(alphabetSize, period);

    std::vector<Node> forbiddenNodes = { Node("012", 0) };

    Graph graph = beal.generate(forbiddenNodes);

    // ノード数とエッジ数を確認
    EXPECT_EQ(graph.getNodes().size(), 5); // 4 + 1 = 5
    EXPECT_EQ(graph.getEdges().size(), 12); // (5 - 1) * 3 = 12

    // 禁止ノードからエッジが始まらないことを確認
    for (const auto& edge : graph.getEdges()) {
        EXPECT_NE(edge.getSource(), Node("012", 0));
    }
}
