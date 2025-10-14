#pragma once

#include "../core/Graph.hpp"
#include "../core/Edge.hpp"
#include "../core/Node.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace Moore {
    Graph apply(const Graph& graph);

    namespace {
        void initIDs(
            const std::unordered_map<Node, std::unordered_map<std::string, Node>>& adjList,
            std::unordered_map<std::string, std::unordered_set<Node>>& toNodes, std::unordered_map<Node, std::string>& toId);
        void updateIDs(
            const std::unordered_map<Node, std::unordered_map<std::string, Node>>& adjList,
            const std::unordered_map<std::string, std::unordered_set<Node>>& toNodes, const std::unordered_map<Node, std::string>& toId,
            std::unordered_map<std::string, std::unordered_set<Node>>& toNewNodes, std::unordered_map<Node, std::string>& toNewId);
    }
};
