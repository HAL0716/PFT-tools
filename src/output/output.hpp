#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "core/Edge.hpp"
#include "core/Node.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// プロジェクトのルートディレクトリを取得する関数
std::filesystem::path getRootPath();

// 出力ディレクトリパスを生成する関数
std::string genOutputPath(const json& config);

// エッジを処理して保存する関数
void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
               const std::vector<Edge>& edges);

// 隣接行列を処理して保存する関数
void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
                         const std::vector<Node>& nodes, const std::vector<Edge>& edges);
