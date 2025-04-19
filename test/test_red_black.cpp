#include <gtest.h>
#include <red_black_table.h>
#include <map>
#include <fstream>

TEST(RedBlackTreeTest, create) {
    EXPECT_NO_THROW((RedBlackTree<int, int>()));
}

TEST(RedBlackTreeTest, put_and_find) {
    RedBlackTree<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_EQ(table.get(1), 10);
    EXPECT_EQ(table.get(2), 20);
}


TEST(RedBlackTreeTest, put_equal_and_find) {
    RedBlackTree<int, int> table;
    table.put(1, -1);
    table.put(2, -2);
    EXPECT_EQ(table.get(2), -2);
    table.put(2, -3);
    EXPECT_EQ(table.get(1), -1);
    EXPECT_EQ(table.get(2), -3);
}

TEST(RedBlackTreeTest, put_and_extract) {
    RedBlackTree<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_NO_THROW(table.extract(1));
    EXPECT_ANY_THROW(table.get(1));
}

TEST(RedBlackTreeTest, put_and_contains) {
    RedBlackTree<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(2));
    EXPECT_FALSE(table.contains(3));
}

TEST(RedBlackTreeTest, big_random_test) {
    std::ofstream big_test_output("big_test_output.txt");
    RedBlackTree<int, int> table(big_test_output);
    std::map<int, int> pairs;
    for (int i = 0; i < 10000; ++i) {
        int key = rand() % 10000;
        int value = rand() % 10000;
        pairs[key] = value;
        table.put(key, value);
        EXPECT_TRUE(table.contains(key));
    }
    for (auto& i : pairs) {
        int key = i.first;
        int value = i.second;
        table.extract(key);
        EXPECT_FALSE(table.contains(key));
    }
}