#pragma once

#include <string>
#include <utility>  // for std::pair
#include <vector>

#include "core/Graph.hpp"
#include "core/Node.hpp"
#include "Config.hpp"

namespace io::input {

using Config = io::formats::json::Config;

/**
 * @brief Load configuration from a JSON file.
 *
 * @param filePath Path to the JSON configuration file.
 * @param config Reference to the Config object to populate.
 * @return true if the configuration is successfully loaded, false otherwise.
 */
bool config(const std::string& filePath, Config& config);

/**
 * @brief Load edges from a CSV file and populate the graph.
 *
 * @param filePath Path to the CSV file containing edge data.
 * @param graph Reference to the Graph object to populate.
 * @return true if the edges are successfully loaded, false otherwise.
 */
bool edges(const std::string& filePath, Graph& graph);

/**
 * @brief Load an adjacency matrix from a CSV file and populate the graph.
 *
 * @param filePath Path to the CSV file containing the adjacency matrix.
 * @param graph Reference to the Graph object to populate.
 * @return true if the adjacency matrix is successfully loaded, false otherwise.
 */
bool adjacencyMatrix(const std::string& filePath, Graph& graph);

}  // namespace io::input
