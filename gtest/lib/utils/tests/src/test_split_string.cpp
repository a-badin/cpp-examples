#include <gtest/gtest.h>
#include <vector>
#include "utils.hpp"

TEST(SplitStringTest, BasicCase) {
    std::vector<std::string> actual = SplitString("");
    std::vector<std::string> expected;
    EXPECT_EQ(expected, actual);
}

TEST(SplitStringTest, SingleDelimeter) {
    std::vector<std::string> actual = SplitString("\t");
    std::vector<std::string> expected{"", ""};
    EXPECT_EQ(expected, actual);
}

TEST(SplitStringTest, SingleValueInStartAndDelimeter) {
    std::vector<std::string> actual = SplitString("1\t");
    std::vector<std::string> expected{"1", ""};
    EXPECT_EQ(expected, actual);
}

TEST(SplitStringTest, SingleValueOnEndAndDelimeter) {
    std::vector<std::string> actual = SplitString("\tfoo");
    std::vector<std::string> expected{"", "foo"};
    EXPECT_EQ(expected, actual);
}

