#pragma once

#include <string>
#include <vector>

#include "io/Config.hpp"
#include "core/Node.hpp"

using Config = io::formats::json::Config;

namespace detail {

/**
 * @brief 設定から禁止ノードリストを生成する関数
 *
 * @param config 設定情報
 * @return 生成された禁止ノードリスト
 */
std::vector<std::vector<std::vector<Node>>> genForbiddenList(const Config& config);

/**
 * @brief DFSを用いて禁止ノードを組み合わせる関数
 *
 * @param forbiddenList 各位置ごとの禁止ノードリスト
 * @return 組み合わせた禁止ノード
 */
std::vector<std::vector<Node>> combineNodes(
    const std::vector<std::vector<std::vector<Node>>>& forbiddenList);

}  // namespace detail

/**
 * @brief 設定から禁止ノードを生成する関数
 *
 * @param config 設定情報
 * @return 生成された禁止ノード
 */
std::vector<std::vector<Node>> genNodesFromConfig(const Config& config);
