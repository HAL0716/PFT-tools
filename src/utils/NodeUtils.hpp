#pragma once

#include "../core/Node.hpp"
#include <vector>
#include <algorithm>
#include <iterator>

// ヘルパー関数: ノードがリストに含まれているかをチェック
template <typename Container>
inline bool containsNode(const Container& nodeList, const Node& targetNode) {
    return std::find(std::begin(nodeList), std::end(nodeList), targetNode) != std::end(nodeList);
}
