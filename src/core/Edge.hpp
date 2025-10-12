#pragma once

#include <string>
#include "Node.hpp"

class Edge {
public:
    // コンストラクタ
    Edge(const Node& source, const Node& target, const std::string& label = "");

    // ゲッター
    const Node& getSource() const;
    const Node& getTarget() const;
    const std::string& getLabel() const;

    // 比較演算子
    bool operator==(const Edge& other) const;
    bool operator<(const Edge& other) const;

    // ストリーム出力演算子
    friend std::ostream& operator<<(std::ostream& os, const Edge& edge);

private:
    Node source; // 始点ノード
    Node target; // 終点ノード
    std::string label; // エッジのラベル
};
