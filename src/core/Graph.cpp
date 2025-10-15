#include "Graph.hpp"

// ノードを追加
void Graph::addNode(const Node& node) {
    nodes.push_back(node);
}

// エッジを追加
void Graph::addEdge(const Edge& edge) {
    edges.push_back(edge);
}

// ノードリストを取得
const std::vector<Node>& Graph::getNodes() const {
    return nodes;
}

// エッジリストを取得
const std::vector<Edge>& Graph::getEdges() const {
    return edges;
}

// 隣接リストを生成
std::unordered_map<Node, std::unordered_map<std::string, Node>> Graph::genAdjacencyList() const {
    std::unordered_map<Node, std::unordered_map<std::string, Node>> adjList;
    for (const auto& edge : edges) {
        adjList[edge.getSource()][edge.getLabel()] = edge.getTarget();
    }
    return adjList;
}
