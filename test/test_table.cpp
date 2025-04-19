#include "gtest.h"
#include <table.h>
#include <fstream>

TEST(TableTest, create) {
    EXPECT_NO_THROW((Table<int, int>()));
}

TEST(TableTest, put_and_find) {
    Table<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_EQ(table.get(1), 10);
    EXPECT_EQ(table.get(2), 20);
}

TEST(TableTest, put_and_extract) {
    Table<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_NO_THROW(table.extract(1));
    EXPECT_ANY_THROW(table.get(1));
}

TEST(TableTest, put_and_contains) {
    Table<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(2));
    EXPECT_FALSE(table.contains(3));
}

TEST(TableTest, big_test) {
    std::ofstream big_test_output("big_test_output.txt");
    Table<int, int> table(big_test_output);
    for (int i = 0; i < 10000; ++i) {
        table.put(i, i);
        EXPECT_TRUE(table.contains(i));
    }
    table.put(1000, -1);
    EXPECT_EQ(table.get(1000), -1);
}

TEST(TableTest, get_after_extract) {
    Table<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_NO_THROW(table.extract(1));
    EXPECT_ANY_THROW(table.get(1));
}




