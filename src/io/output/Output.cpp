#include "Output.hpp"

#include <iostream>

#include "io/formats/csv/csv.hpp"
#include "io/formats/csv/format.hpp"

namespace io::output {

namespace {

using CsvData = io::formats::csv::CsvData;

// CSVデータを書き込む共通関数
bool writeCsvData(const std::string& filePath, const CsvData& csvData) {
    if (io::formats::csv::write(filePath, csvData)) {
        std::cout << "Successfully wrote CSV data to " << filePath << std::endl;
        return true;
    } else {
        std::cerr << "Failed to write CSV data to file: " << filePath << std::endl;
        return false;
    }
}
}  // unnamed namespace

// エッジリストを保存する関数
bool edges(const std::string& filePath, const Graph& graph) {
    const auto& csvData = io::formats::csv::format::edges(graph);
    return writeCsvData(filePath, csvData);
}

// 隣接行列を保存する関数
bool adjacencyMatrix(const std::string& filePath, const Graph& graph) {
    const auto& csvData = io::formats::csv::format::adjacencyMatrix(graph);
    return writeCsvData(filePath, csvData);
}

// 許可されたシーケンスを保存する関数
bool sequences(const std::string& filePath, const Graph& graph, unsigned int length) {
    const auto& csvData = io::formats::csv::format::sequences(graph, length);
    return writeCsvData(filePath, csvData);
}

}  // namespace io::output
