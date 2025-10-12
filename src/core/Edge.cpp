#include "Edge.hpp"

// コンストラクタ
Edge::Edge(const Node& source, const Node& target, const std::string& label)
    : source(source), target(target), label(label) {}

// ゲッター
const Node& Edge::getSource() const {
    return source;
}

const Node& Edge::getTarget() const {
    return target;
}

const std::string& Edge::getLabel() const {
    return label;
}

// 比較演算子
bool Edge::operator==(const Edge& other) const {
    return source == other.source && target == other.target && label == other.label;
}

// 比較演算子
bool Edge::operator<(const Edge& other) const {
    if (source != other.source) return source < other.source;
    if (target != other.target) return target < other.target;
    return label < other.label;
}

// ストリーム出力演算子
std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << edge.getSource() << " -" << edge.getLabel() << " -> " << edge.getTarget();
    return os;
}
