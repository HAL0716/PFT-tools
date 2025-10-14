#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "core/Graph.hpp"
#include "Config.hpp"

bool loadConfig(const std::string& filePath, Config& config);

bool loadEdges(const std::string& filePath, Graph& graph);

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph);
