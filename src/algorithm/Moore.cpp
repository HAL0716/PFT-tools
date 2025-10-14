#include "Moore.hpp"
#include "../core/Edge.hpp"
#include <numeric> // For std::accumulate

namespace Moore {
    Graph apply(const Graph& graph) {
        const auto& adjList = graph.genAdjacencyList();

        std::unordered_map<std::string, std::unordered_set<Node>> toNodes;
        std::unordered_map<Node, std::string> toId;
        initIDs(adjList, toNodes, toId);

        std::unordered_map<std::string, std::unordered_set<Node>> toNewNodes;
        std::unordered_map<Node, std::string> toNewId;
        do {
            toNewNodes.clear();
            updateIDs(adjList, toNodes, toId, toNewNodes, toNewId);

            toNodes.swap(toNewNodes);
            toId.swap(toNewId);
        } while (toNewNodes.size() != toNodes.size());

        // ノードマッピング作成
        std::unordered_map<Node, Node> toNewNode;
        for (const auto& [id, nodes] : toNodes) {
            const Node& newNode = *std::min_element(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) { return a < b; });
            for (const auto& oldNode : nodes) {
                toNewNode[oldNode] = newNode;
            }
        }

        // グラフの再構築
        Graph newGraph;
        for (const auto& [_, nodes] : toNodes) {
            const Node& src = *std::min_element(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) { return a < b; });
            newGraph.addNode(src);
            for (const auto& [label, tgt] : adjList.at(src)) {
                newGraph.addEdge(Edge(src, toNewNode.at(tgt), label));
            }
        }

        return newGraph;
    }

    namespace {
        void initIDs(
            const std::unordered_map<Node, std::unordered_map<std::string, Node>>& adjList,
            std::unordered_map<std::string, std::unordered_set<Node>>& toNodes, std::unordered_map<Node, std::string>& toId
        ) {
            for (const auto& [node, edges] : adjList) {
                std::vector<std::string> ids;
                ids.reserve(edges.size());
                for (const auto& [label, _] : edges) {
                    ids.push_back(label);
                }
                std::sort(ids.begin(), ids.end());
                std::string id = std::accumulate(ids.begin(), ids.end(), std::string());

                toNodes[id].emplace(node);
                toId[node] = id;
            }
        }

        void updateIDs(
            const std::unordered_map<Node, std::unordered_map<std::string, Node>>& adjList,
            const std::unordered_map<std::string, std::unordered_set<Node>>& toNodes, const std::unordered_map<Node, std::string>& toId,
            std::unordered_map<std::string, std::unordered_set<Node>>& toNewNodes, std::unordered_map<Node, std::string>& toNewId
        ) {
            for (const auto& [id, nodes] : toNodes) {
                if (nodes.size() == 1) {
                    toNewNodes[id].insert(nodes.begin(), nodes.end());
                } else {
                    for (const auto& src : nodes) {
                        std::string newId = id;

                        std::vector<std::string> ids;
                        for (const auto& [label, tgt] : adjList.at(src)) {
                            ids.push_back(label + toId.at(tgt));
                        }
                        std::sort(ids.begin(), ids.end());
                        newId += std::accumulate(ids.begin(), ids.end(), newId);

                        toNewNodes[newId].emplace(src);
                        toNewId[src] = newId;
                    }
                }
            }
        }
    }
}
