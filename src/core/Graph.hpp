#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Edge.hpp"
#include "Node.hpp"

class Graph {
   public:
    enum class mode { Normal, ID };

    // ノードを追加
    void addNode(const Node& node);

    // エッジを追加
    void addEdge(const Edge& edge);

    // ノードリストを取得
    const std::vector<Node>& getNodes() const;

    // エッジリストを取得
    const std::vector<Edge>& getEdges(const mode& mode = mode::Normal) const;

    // 隣接リストを生成
    std::unordered_map<Node, std::unordered_map<std::string, Node>> genAdjacencyList() const;

    // 長さLの経路の数を計算
    int countPathsOfLength(int length) const;

    // 長さLのエッジラベル列の集合を取得
    std::unordered_set<std::string> getEdgeLabelSequences(int length) const;

   private:
    std::vector<Node> nodes;            // ノードリスト
    std::vector<Edge> edges;            // エッジリスト
    mutable std::vector<Edge> idEdges;  // IDモード用のエッジキャッシュ
};
