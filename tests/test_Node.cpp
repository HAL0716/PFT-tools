#include "gtest/gtest.h"
#include "core/Node.hpp"

// Node クラスのテスト

TEST(NodeTest, ConstructorAndGetters) {
    Node node("A", 1);
    EXPECT_EQ(node.getLabel(), "A");
    EXPECT_EQ(node.getPhase(), 1);
}

TEST(NodeTest, EqualityOperator) {
    Node node1("A", 1);
    Node node2("A", 1);
    Node node3("B", 2);

    EXPECT_TRUE(node1 == node2);
    EXPECT_FALSE(node1 == node3);
}
