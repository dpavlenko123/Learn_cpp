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


TEST_F(VectorTest, Iterator) {
    MyVector<int> vec{1, 2, 3, 4, 5};

    int expected = 1;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        EXPECT_EQ(expected++, *it);
    }
}

TEST_F(VectorTest, ConstIterator) {
    const MyVector<int> vec{1, 2, 3, 4, 5};

    int expected = 1;
    for (MyVector<int>::const_iterator it = vec.cbegin(); it != vec.cend(); ++it) {
        EXPECT_EQ(expected++, *it);
    }
}

TEST_F(VectorTest, IteratorModify) {
    MyVector<int> vec{1, 2, 3};

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        *it *= 2;
    }

    EXPECT_EQ(2, vec[0]);
    EXPECT_EQ(4, vec[1]);
    EXPECT_EQ(6, vec[2]);
}

TEST_F(VectorTest, ReverseIterator) {
    MyVector<int> vec{1, 2, 3, 4};

    int expected = 4;
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        EXPECT_EQ(expected--, *it);
    }
}

TEST_F(VectorTest, ConstReverseIterator) {
    const MyVector<int> vec{1, 2, 3, 4};

    int expected = 4;
    for (auto it = vec.crbegin(); it != vec.crend(); ++it) {
        EXPECT_EQ(expected--, *it);
    }
}

TEST_F(VectorTest, IteratorArithmetic) {
    MyVector<int> vec{10, 20, 30, 40, 50};

    auto it = vec.begin();

    EXPECT_EQ(10, *it);

    it += 2;
    EXPECT_EQ(30, *it);

    it = it + 2;
    EXPECT_EQ(50, *it);

    it -= 3;
    EXPECT_EQ(20, *it);
}

TEST_F(VectorTest, IteratorComparison) {
    MyVector<int> vec{1, 2, 3};

    auto it1 = vec.begin();
    auto it2 = vec.begin();
    auto it3 = vec.end();

    EXPECT_TRUE(it1 == it2);
    EXPECT_TRUE(it1 != it3);

    ++it2;
    EXPECT_TRUE(it1 != it2);
    EXPECT_TRUE(it2 < it3);
}

TEST_F(VectorTest, IteratorDistance) {
    MyVector<int> vec{1, 2, 3, 4, 5};

    auto dist = std::distance(vec.begin(), vec.end());
    EXPECT_EQ(5, dist);
}

TEST_F(VectorTest, IteratorWithAlgorithm) {
    MyVector<int> v{5, 3, 4, 1, 2};
    std::sort(v.begin(), v.end());

    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST_F(VectorTest, ConstIteratorWithSTLAlgorithm) {
    const MyVector<int> v{10, 20, 30};
    int sum = 0;
    std::for_each(v.cbegin(), v.cend(), [&sum](int val) { sum += val; });

    EXPECT_EQ(sum, 60);
}

TEST_F(VectorTest, ReverseIteratorArithmetic) {
    MyVector<int> v{1, 2, 3, 4};
    auto rit = v.rbegin();
    EXPECT_EQ(*rit, 4);
    rit += 2;
    EXPECT_EQ(*rit, 2);
    rit -= 1;
    EXPECT_EQ(*rit, 3);
}

TEST(StepRangeTest, StepOne) {
    MyVector<int> v{1, 2, 3, 4, 5};
    step_range sr(v, 1);

    int expected = 1;
    for (auto it = sr.begin(); it != sr.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST(StepRangeTest, StepTwo) {
    MyVector<int> v{10, 20, 30, 40, 50};
    step_range sr(v, 2);

    std::vector<int> expected{10, 30, 50};
    size_t idx = 0;
    for (auto it = sr.begin(); it != sr.end(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }
}