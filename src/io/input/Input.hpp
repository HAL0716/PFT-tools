#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../formats/json/Config.hpp"
#include "../../core/Graph.hpp"

bool loadConfig(const std::string& filePath, Config& config);

bool loadEdges(const std::string& filePath, Graph& graph);

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph);
