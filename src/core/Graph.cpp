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
