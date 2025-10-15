#include "gtest/gtest.h"
#include "core/Graph.hpp"

// Graph クラスのテスト

TEST(GraphTest, AddNode) {
    Graph graph;
    Node node1("Node1", 1);
    Node node2("Node2", 2);

    graph.addNode(node1);
    graph.addNode(node2);

    const auto& nodes = graph.getNodes();
    EXPECT_EQ(nodes.size(), 2);
    EXPECT_EQ(nodes[0], node1);
    EXPECT_EQ(nodes[1], node2);
}

TEST(GraphTest, AddEdge) {
    Graph graph;
    Node node1("Node1", 1);
    Node node2("Node2", 2);
    Edge edge(node1, node2, "EdgeLabel");

    graph.addEdge(edge);

    const auto& edges = graph.getEdges();
    EXPECT_EQ(edges.size(), 1);
    EXPECT_EQ(edges[0], edge);
}
