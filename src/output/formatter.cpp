#include "formatter.hpp"

#include <sstream>
#include <unordered_map>

namespace formatter {

std::string edges(const std::vector<Edge>& edges) {
    std::ostringstream oss;
    oss << "Source,Target,Label\n";
    for (const auto& edge : edges) {
        oss << edge.getSource() << "," << edge.getTarget() << "," << edge.getLabel() << "\n";
    }
    return oss.str();
}

std::string adjacencyMatrix(const std::vector<Node>& nodes, const std::vector<Edge>& edges) {
    size_t n = nodes.size();
    std::unordered_map<Node, size_t> toIdx;
    for (size_t i = 0; i < n; ++i) {
        toIdx[nodes[i]] = i;
    }

    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n, 0));
    for (const auto& edge : edges) {
        size_t srcIdx = toIdx[edge.getSource()];
        size_t tgtIdx = toIdx[edge.getTarget()];
        adjMatrix[srcIdx][tgtIdx] += 1;
    }

    std::ostringstream oss;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (j > 0)
                oss << ",";
            oss << adjMatrix[i][j];
        }
        oss << "\n";
    }
    return oss.str();
}

}  // namespace formatter
