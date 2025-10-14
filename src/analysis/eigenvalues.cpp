#include "eigenvalues.hpp"
#include <Spectra/GenEigsSolver.h>
#include <Spectra/MatOp/DenseGenMatProd.h>
#include <unordered_map>
#include <stdexcept>

// Graphを引数に取り、最大固有値を返す関数
double calculateMaxEigenvalue(const Graph& graph) {
    // ノード数を取得
    const auto& nodes = graph.getNodes();
    int n = nodes.size();

    // ノードをインデックスにマッピング
    std::unordered_map<Node, int> toIdx;
    for (int i = 0; i < n; ++i) {
        toIdx[nodes[i]] = i;
    }

    // 隣接行列を作成
    Eigen::MatrixXd adjacencyMatrix = Eigen::MatrixXd::Zero(n, n);
    for (const auto& edge : graph.getEdges()) {
        int sourceIndex = toIdx[edge.getSource()];
        int targetIndex = toIdx[edge.getTarget()];
        adjacencyMatrix(sourceIndex, targetIndex) += 1;
    }

    // Spectraを使用して最大固有値を計算
    try {
        const int nev = 1;
        const int ncv = std::min(std::max(3, 2 * nev + 1), n - 1);

        Spectra::DenseGenMatProd<double> op(adjacencyMatrix);
        Spectra::GenEigsSolver<Spectra::DenseGenMatProd<double>> solver(op, nev, ncv);
        solver.init();
        int nconv = solver.compute(Spectra::SortRule::LargestReal);

        if (solver.info() == Spectra::CompInfo::Successful && nconv > 0) {
            return solver.eigenvalues()[0].real();
        } else {
            throw std::runtime_error("Spectra failed to compute eigenvalues.");
        }
    } catch (const std::exception& e) {
        // Spectraが失敗した場合、Eigenを使用
        try {
            Eigen::EigenSolver<Eigen::MatrixXd> solver(adjacencyMatrix);
            if (solver.info() == Eigen::Success) {
                return solver.eigenvalues().real().maxCoeff();
            } else {
                throw std::runtime_error("Eigen failed to compute eigenvalues.");
            }
        } catch (const std::exception& eigenEx) {
            throw std::runtime_error(std::string("[Eigen] exception: ") + eigenEx.what());
        }
    }
}
