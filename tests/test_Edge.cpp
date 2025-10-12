#include "gtest/gtest.h"
#include "core/Edge.hpp"

// Edge クラスのテスト

TEST(EdgeTest, ConstructorAndGetters) {
    Node source("Source", 1);
    Node target("Target", 2);
    Edge edge(source, target, "Label");

    EXPECT_EQ(edge.getSource(), source);
    EXPECT_EQ(edge.getTarget(), target);
    EXPECT_EQ(edge.getLabel(), "Label");
}

TEST(EdgeTest, EqualityOperator) {
    Node source1("Source", 1);
    Node target1("Target", 2);
    Edge edge1(source1, target1, "Label");

    Node source2("Source", 1);
    Node target2("Target", 2);
    Edge edge2(source2, target2, "Label");

    Node source3("DifferentSource", 3);
    Edge edge3(source3, target1, "Label");

    EXPECT_TRUE(edge1 == edge2);
    EXPECT_FALSE(edge1 == edge3);
}
