#include "DeBruijn.hpp"
#include "../core/constants.hpp"
#include "../core/Node.hpp"
#include "../core/Graph.hpp"
#include <unordered_set>

// 組み合わせ生成ヘルパー関数
std::vector<std::string> DeBruijn::generateCombinations(unsigned int wordLength) const {
    std::vector<std::string> combinations;

    // 再帰的ラムダ式による組み合わせ生成
    std::function<void(std::string)> generate = [&](std::string current) {
        if (current.size() == wordLength) {
            combinations.push_back(current);
            return;
        }
        for (char c : alphabet) {
            generate(current + c);
        }
    };

    generate("");
    return combinations;
}

// ノード生成の更新
void DeBruijn::generateNodes(unsigned int wordLength, unsigned int period) {
    nodes.clear();
    auto combinations = generateCombinations(wordLength);

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

            if (std::find(nodes.begin(), nodes.end(), nextNode) != nodes.end()) {
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

// ヘルパー関数: ノードが禁止リストに含まれているかをチェック
inline bool isNotForbidden(const Node& node, const std::vector<Node>& forbiddenNodes) {
    return std::find(forbiddenNodes.begin(), forbiddenNodes.end(), node) == forbiddenNodes.end();
}

// グラフ生成
Graph DeBruijn::generate(const std::vector<Node>& forbiddenNodes) const {
    Graph graph;

    for (const auto& node : nodes) {
        // 禁止ノードに含まれていない場合のみ追加
        if (isNotForbidden(node, forbiddenNodes)) {
            graph.addNode(node);
        }
    }

    for (const auto& edge : edges) {
        // 始点と終点が禁止ノードでない場合のみ追加
        if (isNotForbidden(edge.getSource(), forbiddenNodes) && isNotForbidden(edge.getTarget(), forbiddenNodes)) {
            graph.addEdge(edge);
        }
    }

    return graph;
}
