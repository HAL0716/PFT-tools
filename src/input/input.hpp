#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>
#include "core/Graph.hpp"

bool loadConfig(const std::string& configPath, nlohmann::json& config);

bool loadEdges(const std::string& filePath, Graph& graph);

bool loadAdjacencyMatrix(const std::string& filePath, Graph& graph);
