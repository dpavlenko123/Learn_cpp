#include <gtest/gtest.h>
#include "my_vector.hpp"
#include <string>

struct Person {
    int age;
    std::string name;

    Person() = default;
    Person(int age_, const std::string& name_)
        : age(age_), name(name_) {}
};

class VectorTest : public ::testing::Test {
protected:
    MyVector<int> v1;
    MyVector<double> v2 = MyVector<double>(3, 5.0);
    MyVector<std::string> v3{"1", "2", "3", "4"};
    MyVector<Person> v4;
};

TEST_F(VectorTest, EmptyVector) {
    EXPECT_EQ(0, v1.size());
    EXPECT_EQ(0, v1.capacity());
    EXPECT_TRUE(v1.empty());
}

TEST_F(VectorTest, VectorWithValue) {
    EXPECT_EQ(3, v2.size());
    EXPECT_EQ(3, v2.capacity());

    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(5.0, v2[i]);
    }
}

TEST_F(VectorTest, VectorInitList) {
    EXPECT_EQ(4, v3.size());
    EXPECT_EQ(4, v3.capacity());

    std::string list[] = {"1", "2", "3", "4"};
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_EQ(list[i], v3[i]);
    }
}

TEST_F(VectorTest, CopyConstructor) {
    MyVector<double> copy(v2);

    EXPECT_EQ(v2.size(), copy.size());
    EXPECT_EQ(v2.capacity(), copy.capacity());

    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(v2[i], copy[i]);
    }

    copy[0] = 4;
    EXPECT_EQ(4, copy[0]);
    EXPECT_EQ(5, v2[0]);
}

TEST_F(VectorTest, MoveConstructor) {
    std::string* ptr = &v3[0];

    MyVector<std::string> move(std::move(v3));

    EXPECT_EQ(&move[0], ptr);
    EXPECT_EQ(4, move.size());
    EXPECT_EQ(4, move.capacity());

    std::string list[] = {"1", "2", "3", "4"};
    for (size_t i = 0; i < move.size(); ++i) {
        EXPECT_EQ(list[i], move[i]);
    }

    EXPECT_TRUE(v3.empty());
}

TEST_F(VectorTest, CopyOperator) {
    MyVector<double> copy = v2;

    EXPECT_EQ(v2.size(), copy.size());
    EXPECT_EQ(v2.capacity(), copy.capacity());

    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(v2[i], copy[i]);
    }

    copy[0] = 4;
    EXPECT_EQ(4, copy[0]);
    EXPECT_EQ(5, v2[0]);
}

TEST_F(VectorTest, MoveOperator) {
    std::string* ptr = &v3[0];

    MyVector<std::string> move = std::move(v3);

    EXPECT_EQ(&move[0], ptr);
    EXPECT_EQ(4, move.size());
    EXPECT_EQ(4, move.capacity());

    std::string list[] = {"1", "2", "3", "4"};
    for (size_t i = 0; i < move.size(); ++i) {
        EXPECT_EQ(list[i], move[i]);
    }

    EXPECT_TRUE(v3.empty());
}

TEST_F(VectorTest, PushBackFromEmpty) {
    v1.push_back(1);

    EXPECT_EQ(1, v1.capacity());
    EXPECT_EQ(1, v1.size());
    EXPECT_EQ(1, v1[0]);
}

TEST_F(VectorTest, PushBackMany) {
    for (size_t i = 0; i < 10; ++i) {
        v1.push_back(i);
    }

    EXPECT_EQ(10, v1.size());
    EXPECT_EQ(16, v1.capacity());
}

TEST_F(VectorTest, PushBackMove) {
    std::string str = "heavy string";
    v3.push_back(std::move(str));

    EXPECT_TRUE(str.empty());
}

TEST_F(VectorTest, EmplaceBack) {
    v4.emplace_back(19, "Dana");

    EXPECT_EQ(19, v4[0].age);
    EXPECT_EQ("Dana", v4[0].name);
}

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }