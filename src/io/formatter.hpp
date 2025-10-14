#pragma once

#include <string>
#include <vector>
#include "core/Edge.hpp"
#include "core/Node.hpp"

namespace formatter {
    // EdgesをCSV形式の文字列に変換する関数
    std::string edges(const std::vector<Edge>& edges);

    // 隣接行列をCSV形式の文字列に変換する関数
    std::string adjacencyMatrix(const std::vector<Node>& nodes, const std::vector<Edge>& edges);
}
