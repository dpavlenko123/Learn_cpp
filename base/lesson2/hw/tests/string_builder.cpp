#include <gtest/gtest.h>
#include "string_builder.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <string>

// ---------- Constructors: single values ----------

TEST(StringBuilderTest, NumericConstructor) {
    StringBuilder sb(42);
    EXPECT_EQ(sb.str(), "42");
    EXPECT_EQ(sb.size(), 2u);
}

TEST(StringBuilderTest, StringConstructor) {
    std::string s = "hello";
    StringBuilder sb(s);
    EXPECT_EQ(sb.str(), "hello");
}

TEST(StringBuilderTest, ConstCharConstructor) {
    StringBuilder sb("world");
    EXPECT_EQ(sb.str(), "world");
}

// ---------- Constructors: numeric containers ----------

TEST(StringBuilderTest, NumericContainerNoDelimiter) {
    std::vector<int> v{1, 2, 3};
    StringBuilder sb(v);
    EXPECT_EQ(sb.str(), "123");
}

TEST(StringBuilderTest, NumericContainerWithDelimiter) {
    std::vector<int> v{1, 2, 3};
    StringBuilder sb(v, ",");
    EXPECT_EQ(sb.str(), "1,2,3");
}

// ---------- Constructors: string containers ----------

// TEST(StringBuilderTest, StringContainerNoDelimiter) {
//     std::vector<std::string> v{"a", "b", "c"};
//     StringBuilder sb(v);
//     EXPECT_EQ(sb.str(), "abc");
// }

// TEST(StringBuilderTest, StringContainerWithDelimiter) {
//     std::vector<std::string> v{"a", "b", "c"};
//     StringBuilder sb(v, "-");
//     EXPECT_EQ(sb.str(), "a-b-c");
// }

// ---------- Operator+ ----------

TEST(StringBuilderTest, OperatorPlus) {
    StringBuilder a("hello");
    StringBuilder b("world");
    StringBuilder c = a + b;
    EXPECT_EQ(c.str(), "helloworld");
}

// ---------- Append ----------

TEST(StringBuilderTest, AppendNumeric) {
    StringBuilder sb("value=");
    sb.append(10);
    EXPECT_EQ(sb.str(), "value=10");
}

TEST(StringBuilderTest, AppendString) {
    StringBuilder sb("hello");
    sb.append(std::string(" world"));
    EXPECT_EQ(sb.str(), "hello world");
}

TEST(StringBuilderTest, AppendContainer) {
    std::vector<int> v{1,2,3};
    StringBuilder sb("nums:");
    sb.append(v);
    EXPECT_EQ(sb.str(), "nums:123");
}

// ---------- Reverse ----------

TEST(StringBuilderTest, ReverseNormal) {
    StringBuilder sb("abcd");
    sb.reverse();
    EXPECT_EQ(sb.str(), "dcba");
}

TEST(StringBuilderTest, ReverseSingleChar) {
    StringBuilder sb("x");
    sb.reverse();
    EXPECT_EQ(sb.str(), "x");
}

TEST(StringBuilderTest, ReverseEmpty) {
    StringBuilder sb("");
    sb.reverse();
    EXPECT_EQ(sb.str(), "");
}

// ---------- Stream operator ----------

TEST(StringBuilderTest, OstreamOperator) {
    StringBuilder sb("test");
    std::ostringstream oss;
    oss << sb;
    EXPECT_EQ(oss.str(), "test");
}

TEST(StringBuilderTest, memory_lick) {
    int* a = new int(5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}