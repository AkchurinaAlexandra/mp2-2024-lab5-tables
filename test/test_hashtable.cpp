#include <gtest.h>
#include <hash_table.h>

TEST(HashTableTest, create) {
    EXPECT_NO_THROW((HashTable<int, int>(10)));
}

TEST(HashTableTest, put_and_find) {
    HashTable<int, int> table(10);
    table.put(1, 10);
    table.put(2, 20);
    EXPECT_EQ(table.get(1), 10);
    EXPECT_EQ(table.get(2), 20);
}

TEST(HashTableTest, put_overwrite) {
    HashTable<int, int> table(10);
    table.put(1, 10);
    table.put(1, 20);
    EXPECT_EQ(table.get(1), 20);
}