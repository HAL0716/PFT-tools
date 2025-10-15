#include "Graph.hpp"

#include <queue>
#include <unordered_set>

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

// 長さLの経路の数を計算
int Graph::countPathsOfLength(int length) const {
    if (length <= 0)
        return 0;

    auto adjList = genAdjacencyList();
    int pathCount = 0;

    for (const auto& node : nodes) {
        std::queue<std::pair<Node, int>> queue;
        queue.push({node, 0});

        while (!queue.empty()) {
            auto [currentNode, currentLength] = queue.front();
            queue.pop();

            if (currentLength == length) {
                pathCount++;
                continue;
            }

            if (adjList.find(currentNode) != adjList.end()) {
                for (const auto& [_, neighbor] : adjList[currentNode]) {
                    queue.push({neighbor, currentLength + 1});
                }
            }
        }
    }

    return pathCount;
}

// 辺のラベルを繋げてできる指定された長さの系列の集合を取得
// 注意: このアルゴリズムは指数時間計算量を持ち、大きなグラフや長い系列長に対してはメモリや計算時間が膨大になる可能性があります。
// 必要に応じてlengthやグラフサイズに制限を設けてください。
std::unordered_set<std::string> Graph::getEdgeLabelSequences(int length) const {
    if (length <= 0)
        return {};

    auto adjList = genAdjacencyList();
    std::unordered_set<std::string> sequences;

    for (const auto& node : nodes) {
        std::queue<std::tuple<Node, std::string, int>> queue;
        queue.push({node, "", 0});

        while (!queue.empty()) {
            auto [currentNode, currentSequence, currentLength] = queue.front();
            queue.pop();

            if (adjList.find(currentNode) != adjList.end()) {
                for (const auto& [label, neighbor] : adjList[currentNode]) {
                    int nextLength = currentLength + 1;
                    if (nextLength == length) {
                        sequences.insert(currentSequence + label);
                    } else if (nextLength < length) {
                        queue.push({neighbor, currentSequence + label, nextLength});
                    }
                }
            }
        }
    }

    return sequences;
}
