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

// ユーティリティ関数
bool write(const std::string& path, const std::string& data) {
    path::genDirectory(path);
    std::ofstream file(path);
    if (!io::utils::checkFileOpen(file, path)) {
        io::utils::printErrorAndExit("Failed to open file for writing: " + path);
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
        std::cerr << "Error executing command: " << cmd << std::endl;
        return false;
    }
    return true;
}

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
            matrix[src][tgt] += 1;
        } catch (const std::exception& e) {
            throw std::invalid_argument("Node labels must be valid integers for ID mode.");
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

// JSON関連
bool writeJson(const std::string& path, const json& data) {
    return write(path, data.dump(4));
}

// Graphviz関連
bool writeDot(const std::string& path, const Graph& graph) {
    std::ostringstream oss;
    oss << "digraph G {\n";
    for (size_t i = 0; i < graph.getNodes().size(); ++i) {
        oss << "  " << i << " [label=\"" << graph.getNodes()[i].getLabel() << "\"];\n";
    }
    for (const auto& edge : graph.getEdges(Graph::mode::ID)) {
        oss << "  " << edge.getSource() << " -> " << edge.getTarget() << ";\n";
    }
    oss << "}";
    return write(path, oss.str());
}

bool dotToTeX(const std::string& dir, const std::vector<Node>& nodes) {
    auto quote = [](const std::string& str) { return "\"" + str + "\""; };
    std::string dotPath = path::genFilePath(dir, nodes, "dot", "dot");
    std::string texPath = path::genFilePath(dir, nodes, "tex", "tex");
    const std::string pythonPath = std::string(PYTHON_VENV_PATH) + "/bin/python3";
    const std::string script =
        (std::filesystem::path(PROJECT_SOURCE_DIR) / "scripts" / "convert_dot_to_tex.py").string();
    const std::string cmd =
        quote(pythonPath) + " " + quote(script) + " " + quote(dotPath) + " " + quote(texPath);
    return exec(cmd);
}

bool texToPDF(const std::string& dir, const std::vector<Node>& nodes) {
    auto quote = [](const std::string& str) { return "\"" + str + "\""; };
    std::string texPath = path::genFilePath(dir, nodes, "tex", "tex");
    std::string pdfPath = path::genFilePath(dir, nodes, "pdf", "pdf");
    const std::string cmd = "pdflatex -interaction=nonstopmode -output-directory=" +
                            quote(path::getDirectory(pdfPath)) + " " + quote(texPath) +
                            " > /dev/null 2>&1";
    return exec(cmd);
}

bool pdfToPNG(const std::string& dir, const std::vector<Node>& nodes) {
    auto quote = [](const std::string& str) { return "\"" + str + "\""; };
    std::string pdfPath = path::genFilePath(dir, nodes, "pdf", "pdf");
    std::string pngPath = path::genFilePath(dir, nodes, "png", "png");
    const std::string cmd = "convert -density 300 " + quote(pdfPath) + " " + quote(pngPath);
    return exec(cmd);
}
