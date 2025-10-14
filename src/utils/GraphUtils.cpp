#include "GraphUtils.hpp"

#include "NodeUtils.hpp"

// 計算: ノードの出次数と入次数
std::pair<std::unordered_map<Node, int>, std::unordered_map<Node, int>> calcDegrees(
    const Graph& graph) {
    const auto& edges = graph.getEdges();
    const auto& nodes = graph.getNodes();

    std::unordered_map<Node, int> outDeg;
    std::unordered_map<Node, int> inDeg;

    for (const auto& node : nodes) {
        outDeg[node] = 0;
        inDeg[node] = 0;
    }

    for (const auto& edge : edges) {
        outDeg[edge.getSource()]++;
        inDeg[edge.getTarget()]++;
    }

    return {outDeg, inDeg};
}

// 削除: 孤立ノードを削除
std::unordered_set<Node> removeZeroDegNodes(const Graph& graph,
                                            std::unordered_map<Node, int>& outDeg,
                                            std::unordered_map<Node, int>& inDeg) {
    const auto& edges = graph.getEdges();
    const auto& nodes = graph.getNodes();

    std::unordered_set<Node> removed;
    std::queue<Node> zeroDegNodes;

    for (const auto& node : nodes) {
        if (outDeg[node] == 0 || inDeg[node] == 0) {
            zeroDegNodes.push(node);
            removed.insert(node);
        }
    }

    while (!zeroDegNodes.empty()) {
        Node current = zeroDegNodes.front();
        zeroDegNodes.pop();

        for (const auto& edge : edges) {
            if (edge.getSource() == current) {
                Node tgt = edge.getTarget();
                inDeg[tgt]--;
                if (inDeg[tgt] == 0 && !containsNode(removed, tgt)) {
                    zeroDegNodes.push(tgt);
                    removed.insert(tgt);
                }
            }

            if (edge.getTarget() == current) {
                Node src = edge.getSource();
                outDeg[src]--;
                if (outDeg[src] == 0 && !containsNode(removed, src)) {
                    zeroDegNodes.push(src);
                    removed.insert(src);
                }
            }
        }
    }

    return removed;
}

// 構築: 新しいグラフを生成
Graph buildGraph(const Graph& graph, const std::unordered_set<Node>& removed) {
    const auto& edges = graph.getEdges();
    const auto& nodes = graph.getNodes();

    Graph newGraph;
    for (const auto& node : nodes) {
        if (!containsNode(removed, node)) {
            newGraph.addNode(node);
        }
    }

    for (const auto& edge : edges) {
        if (!containsNode(removed, edge.getSource()) && !containsNode(removed, edge.getTarget())) {
            newGraph.addEdge(edge);
        }
    }

    return newGraph;
}

// メイン関数: 孤立ノードを削除したグラフを生成
Graph cleanGraph(const Graph& graph) {
    auto [outDeg, inDeg] = calcDegrees(graph);
    auto removed = removeZeroDegNodes(graph, outDeg, inDeg);
    return buildGraph(graph, removed);
}
