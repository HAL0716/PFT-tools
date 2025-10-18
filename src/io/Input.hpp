#pragma once

#include <string>
#include <vector>

#include "Config.hpp"
#include "core/Graph.hpp"
#include "core/Node.hpp"

using Config = io::formats::json::Config;

// CSVファイルを読み込む関数
bool readCsv(const std::string& filePath, std::vector<std::vector<std::string>>& csvData);

// JSONファイルを読み込む関数
bool readJson(const std::string& filePath, nlohmann::json& jsonData);

// Config関連
bool readConfigJson(const std::string& filePath, Config& config);

// Edges関連
bool readEdgesCSV(const std::string& filePath, Graph& graph);

// Adjacency Matrix関連
bool readMatrixCSV(const std::string& filePath, Graph& graph);
