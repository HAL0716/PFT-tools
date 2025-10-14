#pragma once

#include <string>
#include <vector>
#include "core/Edge.hpp"
#include "core/Node.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// EdgesをCSV形式の文字列に変換する関数
std::string formatEdges(const std::vector<Edge>& edges);

// 隣接行列をCSV形式の文字列に変換する関数
std::string formatAdjacencyMatrix(const std::vector<Node>& nodes, const std::vector<Edge>& edges);

// CSV形式の文字列をファイルに保存する関数
void writeCSV(const std::string& filePath, const std::string& csvData);

// プロジェクトのルートディレクトリを取得する関数
std::filesystem::path getRootPath();

// 出力ディレクトリパスを生成する関数
std::string genOutputPath(const json& config);

// エッジを指定されたディレクトリに保存する関数
void saveEdges(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes, const std::vector<Edge>& edges);

// 隣接行列を指定されたディレクトリに保存する関数
void saveAdjacencyMatrix(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes, const std::vector<Node>& nodes, const std::vector<Edge>& edges);
