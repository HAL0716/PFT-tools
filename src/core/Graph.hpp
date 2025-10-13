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

    // 隣接リストを生成
    std::unordered_map<Node, std::unordered_map<std::string, Node>> genAdjacencyList() const;

private:
    std::vector<Node> nodes; // ノードリスト
    std::vector<Edge> edges; // エッジリスト
};
