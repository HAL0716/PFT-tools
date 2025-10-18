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

#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR ""
#endif

namespace graphviz {

namespace {

void writeHeader(std::ostringstream& content) {
    content << "digraph G {\n"
            << "\tlayout=neato;\n"
            << "\tsplines=true;\n"
            << "\tnode [shape=ellipse, fixedsize=true];\n"
            << "\n";
}

void writeVertices(std::ostringstream& content, const std::vector<Node>& nodes) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        content << "\t" << i << " [texlbl=\"$" << nodes[i].toTeX() << "$\"];\n";
    }
}

void writeEdges(std::ostringstream& content, const std::vector<Edge>& edges) {
    for (const auto& edge : edges) {
        content << "\t" << edge.getSource().getLabel() << " -> " << edge.getTarget().getLabel()
                << " [label=\"" << edge.getLabel() << "\", texlbl=\"$" << edge.getLabel()
                << "$\"];\n";
    }
}

}  // namespace

std::string cvtGraph2Dot(const Graph& graph) {
    std::ostringstream content;
    writeHeader(content);
    writeVertices(content, graph.getNodes());
    writeEdges(content, graph.getEdges(Graph::mode::ID));
    content << "}";
    return content.str();
}

// GraphをDOTファイルとして保存する関数
bool saveDot(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes,
             const Graph& graph) {
    std::string filePath = path::genFilePath(baseDirectory, forbiddenNodes, "dot", "dot");

    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
        return false;
    }
    outFile << cvtGraph2Dot(graph);
    outFile.close();
    std::cout << "Saved DOT file: " << filePath << std::endl;
    return true;
}

// ファイル変換関連の関数

bool executeCommand(const std::string& command, const std::string& errorMessage) {
    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << errorMessage << std::endl;
        return false;
    }
    return true;
}

bool cvtDot2TeX(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes) {
    std::string dotFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "dot", "dot");
    std::string texFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "tex", "tex");

    const std::string pythonPath = std::string(PYTHON_VENV_PATH) + "/bin/python3";
    const std::string pythonScript =
        (std::filesystem::path(PROJECT_SOURCE_DIR) / "scripts" / "convert_dot_to_tex.py").string();
    const std::string command = "\"" + pythonPath + "\" \"" + pythonScript + "\" \"" + dotFilePath +
                                "\" \"" + texFilePath + "\"";

    if (!executeCommand(command, "Error: Failed to convert \"" + dotFilePath + "\" to TeX.")) {
        return false;
    }

    std::cout << "Generated TeX file: " << texFilePath << std::endl;
    return true;
}

bool cvtTex2PDF(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes) {
    std::string texFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "tex", "tex");
    std::string pdfFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "pdf", "pdf");

    const std::string command = "pdflatex -interaction=nonstopmode -output-directory=\"" +
                                path::getDirectory(pdfFilePath) + "\" \"" + texFilePath +
                                "\" > /dev/null 2>&1";

    if (!executeCommand(command, "Error: Failed to convert \"" + texFilePath + "\" to PDF.")) {
        return false;
    }

    // 中間ファイルを削除（現在のドキュメントのベース名に限定）
    std::string outputDir = path::getDirectory(pdfFilePath);
    std::string baseName = path::getFileName(pdfFilePath, false);  // 拡張子なし
    const std::vector<std::string> latexAuxExts = {".aux", ".log", ".out", ".toc"};

    for (const auto& ext : latexAuxExts) {
        std::filesystem::path auxPath = std::filesystem::path(outputDir) / (baseName + ext);
        if (std::filesystem::exists(auxPath)) {
            std::filesystem::remove(auxPath);
        }
    }

    std::filesystem::path synctexPath =
        std::filesystem::path(outputDir) / (baseName + ".synctex.gz");
    if (std::filesystem::exists(synctexPath)) {
        std::filesystem::remove(synctexPath);
    }

    std::cout << "Generated PDF file: " << pdfFilePath << std::endl;
    return true;
}

bool cvtPDF2PNG(const std::string& baseDirectory, const std::vector<Node>& forbiddenNodes) {
    std::string pdfFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "pdf", "pdf");
    std::string pngFilePath = path::genFilePath(baseDirectory, forbiddenNodes, "png", "png");

    const std::string baseName = path::getFileName(pdfFilePath, false);
    const std::string outputDir = path::getDirectory(pngFilePath);

    const std::string command = "pdftoppm -png -singlefile \"" + pdfFilePath + "\" \"" + outputDir +
                                "/" + baseName + "\" > /dev/null 2>&1";

    if (!executeCommand(command, "Error: Failed to convert \"" + pdfFilePath + "\" to PNG.")) {
        return false;
    }

    std::cout << "Generated PNG file: " << pngFilePath << std::endl;
    return true;
}

}  // namespace graphviz
