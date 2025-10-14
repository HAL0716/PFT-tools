#pragma once

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/Edge.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"

// 孤立ノードを削除した新しいGraphを生成する関数
Graph cleanGraph(const Graph& graph);

// ヘルパー関数の宣言
std::pair<std::unordered_map<Node, int>, std::unordered_map<Node, int>> calcDegrees(
    const Graph& graph);  // ノードの出次数と入次数を計算
std::unordered_set<Node> removeZeroDegNodes(
    const Graph& graph, std::unordered_map<Node, int>& outDeg,
    std::unordered_map<Node, int>& inDeg);  // 孤立ノードを削除
Graph buildGraph(const Graph& graph,
                 const std::unordered_set<Node>& removed);  // 新しいグラフを構築
