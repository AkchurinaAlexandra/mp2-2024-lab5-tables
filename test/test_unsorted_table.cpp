#include <gtest.h>
#include <table.h>
#include <iostream>
#include <fstream>

TEST(UnsortedTableTest, create) {
    EXPECT_NO_THROW((UnsortedTable<int, int>()));
}

TEST(UnsortedTableTest, put) {
    UnsortedTable<int, int> table;
    EXPECT_NO_THROW(table.put(1, 1));
}

TEST(UnsortedTableTest, put_and_find) {
    UnsortedTable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_EQ(table.get(1), 10);
    EXPECT_EQ(table.get(2), 20);
}

TEST(UnsortedTableTest, put_equal_and_find) {
    UnsortedTable<int, int> table;
    table.put(1, -1);
    table.put(2, -2);
    EXPECT_EQ(table.get(2), -2);
    table.put(2, -3);
    EXPECT_EQ(table.get(1), -1);
    EXPECT_EQ(table.get(2), -3);
}


TEST(UnsortedTableTest, contains) {
    UnsortedTable<int, int> table;
    table.put(1, 10);
    EXPECT_TRUE(table.contains(1));
    EXPECT_FALSE(table.contains(2));
}

TEST(UnsortedTableTest, extract) {
    UnsortedTable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.extract(1);
    EXPECT_FALSE(table.contains(1));
    EXPECT_TRUE(table.contains(2));
}

TEST(UnsortedTableTest, big_test) {
    std::ofstream big_test_output("big_test_output.txt");
    UnsortedTable<int, int> table(big_test_output);
    for (int i = 0; i < 10000; ++i) {
        table.put(i, i);
        EXPECT_TRUE(table.contains(i));
    }
    table.put(1000, -1);
    EXPECT_EQ(table.get(1000), -1);
}