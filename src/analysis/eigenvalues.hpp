#pragma once

#include "../core/Graph.hpp"
#include <Eigen/Dense>

// Graphを引数に取り、最大固有値を返す関数
double calculateMaxEigenvalue(const Graph& graph);
