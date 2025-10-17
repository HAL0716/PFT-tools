#include "Graphviz.hpp"

#include <cstdlib>     // std::system
#include <filesystem>  // std::filesystemを使用
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "path/PathUtils.hpp"

#ifndef PYTHON_VENV_PATH
#define PYTHON_VENV_PATH ""
#endif

namespace graphviz {

// ヘルパー関数: ノードインデックスマッピングを生成
std::unordered_map<Node, size_t> generateNodeIndexMap(const Graph& graph) {
    std::unordered_map<Node, size_t> toIdx;
    auto nodes = graph.getNodes();
    for (size_t i = 0; i < nodes.size(); ++i) {
        toIdx[nodes[i]] = i;
    }
    return toIdx;
}

// DOTファイル関連の関数

void writeHeader(std::ostringstream& content) {
    content << "digraph G {\n"
            << "\tlayout=neato;\n"
            << "\tsplines=true;\n"
            << "\tnode [shape=ellipse, fixedsize=true];\n"
            << "\n";
}

void writeVertices(std::ostringstream& content, const Graph& graph) {
    auto toIdx = generateNodeIndexMap(graph);
    for (const auto& [node, idx] : toIdx) {
        content << "\t" << idx << " [texlbl=\"$" << node.toTeX() << "$\"];\n";
    }
}

void writeEdges(std::ostringstream& content, const Graph& graph) {
    auto toIdx = generateNodeIndexMap(graph);
    for (const auto& edge : graph.getEdges()) {
        content << "\t" << toIdx[edge.getSource()] << " -> " << toIdx[edge.getTarget()]
                << " [label=\"" << edge.getLabel() << "\", texlbl=\"$" << edge.getLabel()
                << "$\"];\n";
    }
}

std::string cvtGraph2Dot(const Graph& graph) {
    std::ostringstream content;
    writeHeader(content);
    writeVertices(content, graph);
    writeEdges(content, graph);
    content << "}";
    return content.str();
}

// GraphをDOTファイルとして保存する関数
void saveDot(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
             const Graph& graph) {
    std::string filePath = path::genFilePath(baseDirectory, forbiddenNodes, "dot", "dot");

    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
        return;
    }
    outFile << cvtGraph2Dot(graph);
    outFile.close();
    std::cout << "Saved DOT file: " << filePath << std::endl;
}

// ファイル変換関連の関数

void executeCommand(const std::string& command, const std::string& errorMessage) {
    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << errorMessage << std::endl;
    }
}

void cvtDot2TeX(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes) {
    std::string dotFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "dot", "dot");
    std::string texFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "tex", "tex");

    const std::string pythonPath = std::string(PYTHON_VENV_PATH) + "/bin/python3";
    const std::string pythonScript = "../scripts/convert_dot_to_tex.py";
    const std::string command =
        pythonPath + " " + pythonScript + " " + dotFilePath + " " + texFilePath;

    executeCommand(command, "Error: Failed to convert " + dotFilePath + " to TeX.");

    std::cout << "Generated TeX file: " << texFilePath << std::endl;
}

void cvtTex2PDF(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes) {
    std::string texFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "tex", "tex");
    std::string pdfFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "pdf", "pdf");

    const std::string command =
        "pdflatex -interaction=nonstopmode -output-directory=" + path::getDirectory(pdfFilePath) +
        " " + texFilePath + " > /dev/null 2>&1";

    executeCommand(command, "Error: Failed to convert " + texFilePath + " to PDF.");

    // 中間ファイルを削除
    std::string outputDir = path::getDirectory(pdfFilePath);
    for (const auto& entry : std::filesystem::directory_iterator(outputDir)) {
        if (entry.path().extension() != ".pdf") {
            std::filesystem::remove(entry.path());
        }
    }

    std::cout << "Generated PDF file: " << pdfFilePath << std::endl;
}

void cvtPDF2PNG(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes) {
    std::string pdfFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "pdf", "pdf");
    std::string pngFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "png", "png");

    const std::string baseName = path::getFileName(pdfFilePath, false);
    const std::string outputDir = path::getDirectory(pngFilePath);
    const std::string command =
        "pdftoppm -png " + pdfFilePath + " " + outputDir + "/" + baseName + " > /dev/null 2>&1";

    executeCommand(command, "Error: Failed to convert " + pdfFilePath + " to PNG.");

    std::cout << "Generated PNG file: " << pngFilePath << std::endl;
}

}  // namespace graphviz
