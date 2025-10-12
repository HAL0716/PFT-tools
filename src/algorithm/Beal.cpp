#include "Beal.hpp"
#include <algorithm>

std::vector<Node> Beal::generateNodes(const std::vector<Node>& forbiddenNodes) const {
    std::vector<Node> nodes;

    for (const auto& node : forbiddenNodes) {
        const std::string& label = node.getLabel();
        unsigned int phase = node.getPhase();

        // 接頭辞ノードを追加
        for (size_t len = 1; len <= label.size(); ++len) {
            nodes.emplace_back(label.substr(0, len), phase);
        }
        // 空系列ノードを追加
        for (size_t p = 0; p < period; ++p) {
            nodes.emplace_back("E", p);
        }
    }

    return nodes;
}

Beal::Beal(unsigned int alphabetSize, unsigned int period) : period(period) {
    alphabet = ALPHABET.substr(0, alphabetSize);
}

Graph Beal::generate(const std::vector<Node>& forbiddenNodes) const {
    Graph graph;
    std::vector<Node> nodes = generateNodes(forbiddenNodes);

    for (const auto& node : nodes) {
        graph.addNode(node);

        // 禁止ノードはスキップ
        if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), node) != forbiddenNodes.end()) {
            continue;
        }

        const std::string& label = node.getLabel();
        unsigned int phase = node.getPhase();

        for (char c : alphabet) {
            std::string nextLabel = (label != "E" ? label : "") + c;

            // 次ノードを探索
            auto findNextNode = [&](const std::string& prefix, unsigned int phaseOffset) -> std::pair<bool, Node> {
                Node candidate(prefix, (phase + phaseOffset) % period);
                if (std::find(nodes.begin(), nodes.end(), candidate) != nodes.end()) {
                    return {true, candidate};
                }
                return {false, Node()};
            };

            bool found = false;
            for (size_t len = 0; len <= nextLabel.size(); ++len) {
                auto [isFound, nextNode] = findNextNode(nextLabel.substr(len), len);
                if (isFound) {
                    graph.addEdge(Edge(node, nextNode, std::string(1, c)));
                    found = true;
                    break;
                }
            }

            // 見つからなかった場合のみ空系列ノードへ遷移
            if (!found) {
                auto [isFound, emptyNode] = findNextNode("E", nextLabel.size());
                if (isFound) {
                    graph.addEdge(Edge(node, emptyNode, std::string(1, c)));
                }
            }
        }
    }

    return graph;
}
