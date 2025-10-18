#pragma once

#include <string>
#include <utility>  // for std::pair
#include <vector>

#include "../formats/json/Config.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"

using Config = io::formats::json::Config;

bool loadConfig(const std::string& filePath, Config& config);

bool loadEdges(const std::string& filePath, Graph& graph);

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph);
