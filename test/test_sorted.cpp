#include <gtest.h>
#include <table.h>

TEST(SortedTableTest, create) {
    EXPECT_NO_THROW((SortedTable<int, int>()));
}

TEST(SortedTableTest, put_and_find) {
    SortedTable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_EQ(table.get(1), 10);
    EXPECT_EQ(table.get(2), 20);
}


TEST(SortedTableTest, put_equal_and_find) {
    SortedTable<int, int> table;
    table.put(1, -1);
    table.put(2, -2);
    EXPECT_EQ(table.get(2), -2);
    table.put(2, -3);
    EXPECT_EQ(table.get(1), -1);
    EXPECT_EQ(table.get(2), -3);
}

TEST(SortedTableTest, put_and_extract) {
    SortedTable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_NO_THROW(table.extract(1));
    EXPECT_ANY_THROW(table.get(1));
}

TEST(SortedTableTest, put_and_contains) {
    SortedTable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(2));
    EXPECT_FALSE(table.contains(3));
}
