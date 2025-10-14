#pragma once

#include <string>
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"

namespace output_helpers {

std::string generateFilePath(const std::string& baseDirectory,
                             const std::vector<Node>& forbiddenNodes,
                             const std::string& subDirectory);

std::string formatEdgesCSV(const Graph& graph);

std::string formatAdjacencyMatrixCSV(const Graph& graph);

}  // namespace output_helpers
