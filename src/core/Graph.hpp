#pragma once

#include <vector>
#include "Node.hpp"
#include "Edge.hpp"

class Graph {
public:
    // ノードを追加
    void addNode(const Node& node);

    // エッジを追加
    void addEdge(const Edge& edge);

    // ノードリストを取得
    const std::vector<Node>& getNodes() const;

    // エッジリストを取得
    const std::vector<Edge>& getEdges() const;

private:
    std::vector<Node> nodes; // ノードリスト
    std::vector<Edge> edges; // エッジリスト
};
