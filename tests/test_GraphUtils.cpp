#include "gtest/gtest.h"
#include "utils/GraphUtils.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "core/Edge.hpp"

// シンクノードがない場合のテスト
TEST(GraphUtilsTest, CleanGraph_NoSinkNodes) {
    Graph graph;

    graph.addNode(Node("A"));
    graph.addNode(Node("B"));
    graph.addNode(Node("C"));

    graph.addEdge(Edge(Node("A"), Node("B")));
    graph.addEdge(Edge(Node("B"), Node("C")));
    graph.addEdge(Edge(Node("C"), Node("A")));

    Graph cleanedGraph = cleanGraph(graph);

    ASSERT_EQ(cleanedGraph.getNodes().size(), 3);
    ASSERT_EQ(cleanedGraph.getEdges().size(), 3);
}

// すべてのノードがシンクノードの場合のテスト
TEST(GraphUtilsTest, CleanGraph_AllSinkNodes) {
    Graph graph;

    graph.addNode(Node("A"));
    graph.addNode(Node("B"));
    graph.addNode(Node("C"));

    ASSERT_EQ(graph.getNodes().size(), 3);
    ASSERT_EQ(graph.getEdges().size(), 0);

    Graph cleanedGraph = cleanGraph(graph);

    ASSERT_EQ(cleanedGraph.getNodes().size(), 0);
    ASSERT_EQ(cleanedGraph.getEdges().size(), 0);
}

// シンクノードを削除するテスト
TEST(GraphUtilsTest, CleanGraph_RemovesSinkNodes) {
    Graph graph;

    graph.addNode(Node("A"));
    graph.addNode(Node("B"));
    graph.addNode(Node("C"));
    graph.addNode(Node("D")); // シンクノード

    graph.addEdge(Edge(Node("A"), Node("B")));
    graph.addEdge(Edge(Node("B"), Node("C")));
    graph.addEdge(Edge(Node("C"), Node("A")));
    graph.addEdge(Edge(Node("C"), Node("D")));

    ASSERT_EQ(graph.getNodes().size(), 4);
    ASSERT_EQ(graph.getEdges().size(), 4);

    Graph cleanedGraph = cleanGraph(graph);

    ASSERT_EQ(cleanedGraph.getNodes().size(), 3);
    ASSERT_EQ(cleanedGraph.getEdges().size(), 3);

    auto nodes = cleanedGraph.getNodes();
    for (const auto& node : nodes) {
        ASSERT_NE(node.getLabel(), "D");
    }
}
