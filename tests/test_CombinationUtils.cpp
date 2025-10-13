#include "utils/CombinationUtils.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

TEST(CombinationUtilsTest, CombineNoRep) {
    std::vector<int> elems = {1, 2, 3};
    std::vector<std::vector<int>> results = combineNoRep(elems, 2);

    std::vector<std::vector<int>> expected = {
        {1, 2}, {1, 3}, {2, 3}
    };

    EXPECT_EQ(results, expected);
}

TEST(CombinationUtilsTest, CombineWithRep) {
    std::vector<int> elems = {1, 2};
    std::vector<std::vector<int>> results = combineWithRep(elems, 2);

    std::vector<std::vector<int>> expected = {
        {1, 1}, {1, 2}, {2, 1}, {2, 2}
    };

    EXPECT_EQ(results, expected);
}

TEST(CombinationUtilsTest, CombineWrapper) {
    std::vector<int> elems = {1, 2};

    // Test without repetition
    std::vector<std::vector<int>> resultsNoRep = combine(elems, 2, false);
    std::vector<std::vector<int>> expectedNoRep = {
        {1, 2}
    };
    EXPECT_EQ(resultsNoRep, expectedNoRep);

    // Test with repetition
    std::vector<std::vector<int>> resultsWithRep = combine(elems, 2, true);
    std::vector<std::vector<int>> expectedWithRep = {
        {1, 1}, {1, 2}, {2, 1}, {2, 2}
    };
    EXPECT_EQ(resultsWithRep, expectedWithRep);
}

TEST(StringCombinationUtilsTest, CombineNoRep) {
    std::string chars = "abc";
    unsigned int length = 2;
    bool withRepetition = false;

    std::vector<std::string> results = combine(chars, length, withRepetition);

    std::vector<std::string> expected = {
        "ab", "ac", "bc"
    };

    EXPECT_EQ(results, expected);
}

TEST(StringCombinationUtilsTest, CombineWithRep) {
    std::string chars = "ab";
    unsigned int length = 2;
    bool withRepetition = true;

    std::vector<std::string> results = combine(chars, length, withRepetition);

    std::vector<std::string> expected = {
        "aa", "ab", "ba", "bb"
    };

    EXPECT_EQ(results, expected);
}
