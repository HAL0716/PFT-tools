#pragma once

#include <string>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"

namespace format {

std::string generateFilePath(const std::string& baseDirectory,
                             const std::vector<Node>& forbiddenNodes,
                             const std::string& subDirectory);

std::string edges(const Graph& graph);

std::string adjacencyMatrix(const Graph& graph);

}  // namespace format
