#include "Output.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "io/utils.hpp"
#include "path/PathUtils.hpp"

#ifndef PYTHON_VENV_PATH
#define PYTHON_VENV_PATH ""
#endif

#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR ""
#endif

namespace io::output {

// ユーティリティ関数
bool write(const std::string& path, const std::string& data) {
    path::genDirectory(path);
    std::ofstream file(path);
    if (!io::utils::checkFileOpen(file, path)) {
        return false;
    }
    try {
        file << data;
    } catch (const std::exception& e) {
        io::utils::printErrorAndExit("Error writing to file: " + path + " - " + e.what());
    }
    return true;
}

bool exec(const std::string& cmd) {
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        std::cerr << "Error executing command: " << cmd << " (Exit code: " << ret << ")"
                  << std::endl;
        return false;
    }
    return true;
}

auto quote = [](const std::string& str) { return "\"" + str + "\""; };

// CSV関連
bool writeCsv(const std::string& path, const CsvData& data) {
    std::ostringstream oss;
    for (const auto& row : data) {
        oss << std::accumulate(row.begin(), row.end(), std::string(),
                               [](const std::string& a, const std::string& b) {
                                   return a.empty() ? b : a + "," + b;
                               })
            << "\n";
    }
    return write(path, oss.str());
}

bool writeEdgesCsv(const std::string& filePath, const Graph& graph) {
    CsvData data;
    for (const auto& edge : graph.getEdges(Graph::mode::ID)) {
        data.push_back({edge.getSource().getLabel(), edge.getTarget().getLabel(), edge.getLabel()});
    }
    return writeCsv(filePath, data);
}

bool writeMatrixCsv(const std::string& filePath, const Graph& graph) {
    auto n = graph.getNodes().size();
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    for (const auto& edge : graph.getEdges(Graph::mode::ID)) {
        try {
            auto src = std::stoi(edge.getSource().getLabel());
            auto tgt = std::stoi(edge.getTarget().getLabel());

            if (src < 0 || src >= n || tgt < 0 || tgt >= n) {
                throw std::out_of_range("Node index out of range");
            }

            matrix[src][tgt] += 1;
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Node labels must be valid integers for ID mode: " +
                                        std::string(e.what()));
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Node index out of range: " + std::string(e.what()));
        }
    }
    CsvData data(n, std::vector<std::string>(n));
    for (size_t i = 0; i < n; ++i) {
        std::transform(matrix[i].begin(), matrix[i].end(), data[i].begin(),
                       [](int val) { return std::to_string(val); });
    }
    return writeCsv(filePath, data);
}

bool writeSeqCsv(const std::string& filePath, const Graph& graph, unsigned int length) {
    auto sequences = graph.getEdgeLabelSequences(length);
    CsvData data;
    for (const auto& seq : sequences) {
        data.push_back({seq});
    }
    return writeCsv(filePath, data);
}

// Graphviz関連
bool writeDot(const std::string& filePath, const Graph& graph) {
    const auto& nodes = graph.getNodes();
    const auto& edges = graph.getEdges(Graph::mode::ID);

    std::ostringstream oss;
    oss << "digraph G {\n"
        << "\tlayout=neato;\n"
        << "\tsplines=true;\n"
        << "\tnode [shape=ellipse, fixedsize=true];\n";
    oss << "\n";
    for (size_t i = 0; i < nodes.size(); ++i) {
        oss << "\t" << i << " [texlbl=\"$" << nodes[i].toTeX() << "$\"];\n";
    }
    oss << "\n";
    for (const auto& edge : edges) {
        oss << "\t" << edge.getSource().getLabel() << " -> " << edge.getTarget().getLabel()
            << " [label=\"" << edge.getLabel() << "\", texlbl=\"$" << edge.getLabel() << "$\"];\n";
    }
    oss << "}";
    return write(filePath, oss.str());
}

bool dotToTeX(const std::string& dotFilePath) {
    const std::string pythonPath = std::string(PYTHON_VENV_PATH) + "/bin/python3";
    const std::string scriptPath =
        (std::filesystem::path(PROJECT_SOURCE_DIR) / "scripts" / "convert_dot_to_tex.py").string();
    const std::string texFilePath =
        path::getDirectory(dotFilePath) + "/" + path::getFileName(dotFilePath, false) + ".tex";

    const std::string cmd = quote(pythonPath) + " " + quote(scriptPath) + " " + quote(dotFilePath) +
                            " " + quote(texFilePath);
    return exec(cmd);
}

bool texToPDF(const std::string& texFilePath) {
    const std::string directory = path::getDirectory(texFilePath);

    const std::string cmd =
        "pdflatex -interaction=nonstopmode -output-directory=" + quote(directory) + " " +
        quote(texFilePath) + " > /dev/null 2>&1";
    return exec(cmd);
}

bool pdfToPNG(const std::string& pdfFilePath) {
    const std::string directory = path::getDirectory(pdfFilePath);
    const std::string baseName = path::getFileName(pdfFilePath, false);

    const std::string cmd = "pdftoppm -png -singlefile " + quote(pdfFilePath) + " " +
                            quote(directory + "/" + baseName) + " > /dev/null 2>&1";
    return exec(cmd);
}

bool writePdf(const std::string& filePath, const Graph& graph) {
    const std::string tempDir = path::getDirectory(filePath) + "/temp";
    path::genDirectory(tempDir);

    const std::string dotFilePath = tempDir + "/" + path::getFileName(filePath, false) + ".dot";
    if (!writeDot(dotFilePath, graph)) {
        return false;
    }

    const std::string texFilePath = tempDir + "/" + path::getFileName(filePath, false) + ".tex";
    if (!dotToTeX(dotFilePath)) {
        return false;
    }

    const std::string tempPdfPath = tempDir + "/" + path::getFileName(filePath, false) + ".pdf";
    if (!texToPDF(texFilePath)) {
        return false;
    }

    const std::string finalPdfPath = filePath;
    std::filesystem::rename(tempPdfPath, finalPdfPath);

    std::filesystem::remove_all(tempDir);

    return true;
}

bool writePng(const std::string& filePath, const Graph& graph) {
    const std::string tempDir = path::getDirectory(filePath) + "/temp";
    path::genDirectory(tempDir);

    const std::string tempPdfPath = tempDir + "/" + path::getFileName(filePath, false) + ".pdf";
    if (!writePdf(tempPdfPath, graph)) {
        return false;
    }

    const std::string tempPngPath = tempDir + "/" + path::getFileName(filePath, false) + ".png";
    if (!pdfToPNG(tempPdfPath)) {
        return false;
    }

    const std::string finalPngPath = filePath;
    std::filesystem::rename(tempPngPath, finalPngPath);

    std::filesystem::remove_all(tempDir);

    return true;
}

}  // namespace io::output
