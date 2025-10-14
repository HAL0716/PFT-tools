#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "../core/Node.hpp"

// ヘルパー関数: ノードがリストに含まれているかをチェック
template <typename Container>
inline bool containsNode(const Container& nodeList, const Node& targetNode) {
    return std::find(std::begin(nodeList), std::end(nodeList), targetNode) != std::end(nodeList);
}
