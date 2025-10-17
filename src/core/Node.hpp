#pragma once

#include <functional>
#include <iostream>
#include <string>

class Node {
   public:
    // デフォルトコンストラクタ
    Node() : label(""), phase(0) {}

    // コンストラクタ
    Node(const std::string& label, unsigned int phase = 0);

    // ゲッター
    const std::string& getLabel() const;
    unsigned int getPhase() const;
    std::string toTeX() const;

    // 比較演算子
    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;
    bool operator<(const Node& other) const;

    // ストリーム出力演算子
    friend std::ostream& operator<<(std::ostream& os, const Node& node);

   private:
    std::string label;   // 頂点のラベル
    unsigned int phase;  // 頂点の位相
};

// ハッシュ関数の定義
namespace std {
template <>
struct hash<Node> {
    std::size_t operator()(const Node& node) const {
        return std::hash<std::string>()(node.getLabel()) ^
               (std::hash<unsigned int>()(node.getPhase()) << 1);
    }
};
}  // namespace std
