#pragma once

#include <vector>

#include "../core/Graph.hpp"
#include "../core/Node.hpp"

class GraphGenerator {
   public:
    virtual ~GraphGenerator() = default;

    // 純粋仮想関数: グラフ生成
    virtual Graph generate(const std::vector<Node>& forbiddenNodes) const = 0;
};
