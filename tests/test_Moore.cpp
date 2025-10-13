#include "../src/algorithm/Moore.hpp"
#include "../src/core/Node.hpp"
#include "../src/core/Graph.hpp"
#include <gtest/gtest.h>

// Moore法の適用テスト1
TEST(MooreTest, Apply1) {
    Graph graph;

    graph.addNode(Node("A"));
    graph.addNode(Node("B"));
    graph.addNode(Node("C"));
    graph.addNode(Node("D"));
    graph.addNode(Node("E"));
    graph.addNode(Node("F"));

    graph.addEdge(Edge(Node("A"), Node("C"), "a"));
    graph.addEdge(Edge(Node("A"), Node("E"), "b"));

    graph.addEdge(Edge(Node("B"), Node("A"), "a"));

    graph.addEdge(Edge(Node("C"), Node("B"), "a"));
    graph.addEdge(Edge(Node("C"), Node("D"), "b"));

    graph.addEdge(Edge(Node("D"), Node("A"), "a"));

    graph.addEdge(Edge(Node("E"), Node("F"), "a"));
    graph.addEdge(Edge(Node("E"), Node("D"), "b"));

    graph.addEdge(Edge(Node("F"), Node("A"), "a"));

    Graph newGraph = Moore::apply(graph);

    // ノード数とエッジ数を確認
    EXPECT_EQ(newGraph.getNodes().size(), 3);
    EXPECT_EQ(newGraph.getEdges().size(), 5);
}

// Moore法の適用テスト2
TEST(MooreTest, Apply2) {
    Graph graph;

    graph.addNode(Node("A"));
    graph.addNode(Node("B"));
    graph.addNode(Node("C"));
    graph.addNode(Node("D"));
    graph.addNode(Node("E"));
    graph.addNode(Node("F"));
    graph.addNode(Node("G"));

    graph.addEdge(Edge(Node("A"), Node("D"), "0"));
    graph.addEdge(Edge(Node("A"), Node("B"), "1"));

    graph.addEdge(Edge(Node("B"), Node("E"), "0"));

    graph.addEdge(Edge(Node("C"), Node("A"), "0"));

    graph.addEdge(Edge(Node("D"), Node("F"), "0"));
    graph.addEdge(Edge(Node("D"), Node("C"), "1"));

    graph.addEdge(Edge(Node("E"), Node("D"), "0"));
    graph.addEdge(Edge(Node("E"), Node("B"), "1"));

    graph.addEdge(Edge(Node("F"), Node("G"), "1"));

    graph.addEdge(Edge(Node("G"), Node("E"), "0"));

    Graph newGraph = Moore::apply(graph);

    // ノード数とエッジ数を確認
    EXPECT_EQ(newGraph.getNodes().size(), 4);
    EXPECT_EQ(newGraph.getEdges().size(), 6);

    for (const auto& node : newGraph.getNodes()) {
        std::cout << node << std::endl;
    }
}
