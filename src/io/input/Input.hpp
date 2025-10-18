#pragma once

#include <string>

#include "core/Graph.hpp"
#include "../formats/json/Config.hpp"

using Config = io::formats::json::Config;

bool loadConfig(const std::string& filePath, Config& config);

bool loadEdges(const std::string& filePath, Graph& graph);

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph);
