#include <table_interface.h>
#include <vector_table.h>
#include <hash_table.h>
#include <red_black_table.h>
#include <cassert>

template<typename K, typename V>
class Table {
 public:
    Table() : out(std::cout) {}
    explicit Table(std::ostream& out) : out(out), unsorted_table(out), sorted_table(out), hash_table(out),
    red_black_tree_table(out) {};

    void put(K key, V value) {
        unsorted_table.put(key, value);
        sorted_table.put(key, value);
        hash_table.put(key, value);
        red_black_tree_table.put(key, value);
    }
    bool contains(K key) const {
        bool res1 = unsorted_table.contains(key);
        bool res2 = sorted_table.contains(key);
        bool res3 = hash_table.contains(key);
        bool res4 = red_black_tree_table.contains(key);
        assert(res1 == res2 && res2 == res3 && res3 == res4); // check all tables have the same result
        return res1;
    }
    V get(K key) const {
        V res1 = unsorted_table.get(key);
        V res2 = sorted_table.get(key);
        V res3 = hash_table.get(key);
        V res4 = red_black_tree_table.get(key);
        assert(res1 == res2 && res2 == res3 && res3 == res4); // check all tables
        return res1;
    }
    void extract(K key) {
        unsorted_table.extract(key);
        sorted_table.extract(key);
        hash_table.extract(key);
        red_black_tree_table.extract(key);
    }
 private:
    std::ostream& out; // log stream
    UnsortedTable<K, V> unsorted_table;
    SortedTable<K, V> sorted_table;
    HashTable<K, V> hash_table;
    RedBlackTree<K, V> red_black_tree_table;
};