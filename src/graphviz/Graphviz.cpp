#include "Graphviz.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "path/PathUtils.hpp"

namespace graphviz {

// 頂点の定義を出力する関数
void writeVertices(std::ostringstream& content) {
    const std::vector<std::string> vertices = {"A", "B", "C"};
    for (const auto& vertex : vertices) {
        content << "\t" << vertex << ";\n";
    }
}

// 辺の定義を出力する関数
void writeEdges(std::ostringstream& content) {
    const std::vector<std::pair<std::string, std::string>> edges = {
        {"A", "B"}, {"B", "C"}, {"C", "A"}};
    for (const auto& edge : edges) {
        content << "\t" << edge.first << " -> " << edge.second << ";\n";
    }
}

// グラフの内容を生成する関数
std::string generateGraphContent(const Graph& graph) {
    std::ostringstream content;
    content << "digraph G {\n";
    writeVertices(content);
    writeEdges(content);
    content << "}";
    return content.str();
}

// ダミーの.dotファイルを保存する関数
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

}  // namespace graphviz
