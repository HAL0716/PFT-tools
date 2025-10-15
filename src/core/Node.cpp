#include "Node.hpp"

#include <iostream>

// コンストラクタ
Node::Node(const std::string& label, unsigned int phase) : label(label), phase(phase) {}

// ゲッター
const std::string& Node::getLabel() const {
    return label;
}

unsigned int Node::getPhase() const {
    return phase;
}

// 比較演算子
bool Node::operator==(const Node& other) const {
    return label == other.label && phase == other.phase;
}

// 不等価演算子
bool Node::operator!=(const Node& other) const {
    return !(*this == other);
}

// less-than 演算子
bool Node::operator<(const Node& other) const {
    if (label != other.label) {
        return label < other.label;
    }
    return phase < other.phase;
}

// ストリーム出力演算子
std::ostream& operator<<(std::ostream& os, const Node& node) {
    os << "(" << node.getLabel() << "," << node.getPhase() << ")";
    return os;
}
