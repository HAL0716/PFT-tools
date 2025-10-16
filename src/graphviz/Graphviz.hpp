#pragma once

#include <string>

#include "core/Graph.hpp"

namespace graphviz {

// GraphをDOTファイルとして保存する関数
void saveDotFile(const std::string& filePath, const Graph& graph);

// ヘッダーを出力する関数
void writeHeader(std::ostringstream& content);

// 頂点の定義を出力する関数
void writeVertices(std::ostringstream& content, const Graph& graph);

// 辺の定義を出力する関数
void writeEdges(std::ostringstream& content, const Graph& graph);

// グラフの内容を生成する関数
std::string generateGraphContent(const Graph& graph);

// DOTファイルをTeXファイルに変換する関数
void cvtDot2Tex(const std::string& dotFilePath, const std::string& texFilePath);

// TeXファイルをPDFファイルに変換する関数
void cvtTex2PDF(const std::string& texFilePath, const std::string& pdfFilePath);

// PDFファイルをPNGファイルに変換する関数
void cvtPDF2PNG(const std::string& pdfFilePath, const std::string& pngFilePath);

}  // namespace graphviz
