#pragma once

#include <Eigen/Dense>

#include "../core/Graph.hpp"

// Graphを引数に取り、最大固有値を返す関数
double calculateMaxEigenvalue(const Graph& graph);
