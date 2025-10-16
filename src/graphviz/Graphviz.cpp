#include "Graphviz.hpp"

#include <cstdlib>  // std::system
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "path/PathUtils.hpp"

#ifndef PYTHON_VENV_PATH
#define PYTHON_VENV_PATH ""
#endif

namespace graphviz {

// ヘッダーを出力する関数
void writeHeader(std::ostringstream& content) {
    content << "digraph G {\n"
            << "\tlayout=neato;\n"
            << "\tsplines=true;\n"
            << "\tnode [shape=ellipse, fixedsize=true];\n"
            << "\n";
}

// 頂点の定義を出力する関数
void writeVertices(std::ostringstream& content, const Graph& graph) {
    auto nodes = graph.getNodes();
    std::unordered_map<Node, size_t> toIdx;
    for (size_t i = 0; i < nodes.size(); ++i) {
        toIdx[nodes[i]] = i;
    }

    for (const auto& node : nodes) {
        content << "\t" << toIdx[node] << " [texlbl=\"$" << node << "$\"];\n";
    }
}

// 辺の定義を出力する関数
void writeEdges(std::ostringstream& content, const Graph& graph) {
    auto nodes = graph.getNodes();
    std::unordered_map<Node, size_t> toIdx;
    for (size_t i = 0; i < nodes.size(); ++i) {
        toIdx[nodes[i]] = i;
    }

    for (const auto& edge : graph.getEdges()) {
        content << "\t" << toIdx[edge.getSource()] << " -> " << toIdx[edge.getTarget()]
                << " [label=\"" << edge.getLabel() << "\", texlbl=\"$" << edge.getLabel()
                << "$\"];\n";
    }
}

// グラフの内容を生成する関数
std::string generateGraphContent(const Graph& graph) {
    std::ostringstream content;
    writeHeader(content);
    writeVertices(content, graph);
    writeEdges(content, graph);
    content << "}";
    return content.str();
}

// GraphをDOTファイルとして保存する関数
void saveDotFile(const std::string& filePath, const Graph& graph) {
    path::genDirectory(filePath);

    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
        return;
    }

    outFile << generateGraphContent(graph);

    outFile.close();
    std::cout << "Dummy .dot file saved to " << filePath << std::endl;
}

void cvtDot2Tex(const std::string& dotFilePath, const std::string& texFilePath) {
    // 仮想環境のPythonを使用してスクリプトを実行
    const std::string pythonPath = std::string(PYTHON_VENV_PATH) + "/bin/python3";
    const std::string pythonScript = "../scripts/convert_dot_to_tex.py";
    const std::string command = pythonPath + " " + pythonScript + " " + dotFilePath;

    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to convert " << dotFilePath << " to TeX." << std::endl;
        return;
    }

    std::cout << "Converted " << dotFilePath << " to " << texFilePath << std::endl;
}

void cvtTex2PDF(const std::string& texFilePath, const std::string& pdfFilePath) {
    // pdflatexコマンドを構築
    const std::string command =
        "pdflatex -interaction=nonstopmode -output-directory=" + path::getDirectory(pdfFilePath) +
        " " + texFilePath + " > /dev/null 2>&1";

    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to convert " << texFilePath << " to PDF." << std::endl;
        return;
    }

    std::cout << "Converted " << texFilePath << " to " << pdfFilePath << std::endl;
}

void cvtPDF2PNG(const std::string& pdfFilePath, const std::string& pngFilePath) {
    // pdftoppmコマンドを構築
    const std::string baseName = path::getFileName(pdfFilePath, false);
    const std::string outputDir = path::getDirectory(pngFilePath);
    const std::string command =
        "pdftoppm -png " + pdfFilePath + " " + outputDir + "/" + baseName + " > /dev/null 2>&1";

    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to convert " << pdfFilePath << " to PNG." << std::endl;
        return;
    }

    std::cout << "Converted " << pdfFilePath << " to " << pngFilePath << std::endl;
}

}  // namespace graphviz
