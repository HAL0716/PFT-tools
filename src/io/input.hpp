#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>
#include "core/Node.hpp"

bool loadConfig(const std::string& configPath, nlohmann::json& config);

std::vector<Node> parseForbiddenNodes(const nlohmann::json& config);
