#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "../../core/Graph.hpp"
#include "../formats/json/Config.hpp"

bool loadConfig(const std::string& filePath, Config& config);

bool loadEdges(const std::string& filePath, Graph& graph);

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph);
