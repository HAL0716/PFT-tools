#include "DeBruijn.hpp"
#include "../core/constants.hpp"
#include "../core/Node.hpp"
#include "../core/Graph.hpp"
#include "../utils/NodeUtils.hpp"
#include "../utils/CombinationUtils.hpp"
#include <unordered_set>

// ノード生成の更新
void DeBruijn::generateNodes(unsigned int wordLength, unsigned int period) {
    nodes.clear();
    auto combinations = combine(alphabet, wordLength, true);

    for (const auto& combination : combinations) {
        for (unsigned int phase = 0; phase < period; ++phase) {
            nodes.emplace_back(combination, phase);
        }
    }
}

// エッジ生成のヘルパー関数
void DeBruijn::generateEdges() {
    edges.clear();

    for (const auto& node : nodes) {
        for (char c : alphabet) {
            std::string nextLabel = node.getLabel().substr(1) + c;
            Node nextNode(nextLabel, (node.getPhase() + 1) % period);

            if (containsNode(nodes, nextNode)) {
                edges.emplace_back(node, nextNode, std::string(1, c));
            }
        }
    }
}

// コンストラクタ
DeBruijn::DeBruijn(unsigned int alphabetSize, unsigned int wordLength, unsigned int period) : period(period) {
    alphabet = ALPHABET.substr(0, alphabetSize);
    generateNodes(wordLength, period);
    generateEdges();
}

// グラフ生成
Graph DeBruijn::generate(const std::vector<Node>& forbiddenNodes) const {
    Graph graph;

    for (const auto& node : nodes) {
        // 禁止ノードに含まれていない場合のみ追加
        if (!containsNode(forbiddenNodes, node)) {
            graph.addNode(node);
        }
    }

    for (const auto& edge : edges) {
        // 始点と終点が禁止ノードでない場合のみ追加
        if (!containsNode(forbiddenNodes, edge.getSource()) && !containsNode(forbiddenNodes, edge.getTarget())) {
            graph.addEdge(edge);
        }
    }

    return graph;
}
