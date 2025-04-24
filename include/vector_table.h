#include <table_interface.h>
#include <vector>
#include <algorithm>

template<typename K, typename V>
class VectorTable : public TableInterface<K, V> {
 public:

    explicit VectorTable(const std::string& table_type) : table_type(table_type) {};
    explicit VectorTable(std::ostream& out, const std::string& table_type) : out(out), table_type(table_type) {}

    void put(K key, V value) override {
        size_t idx = find_key_index(key);
        if (idx == -1) {
            table.emplace_back(key, value);
        } else {
            table[idx] = std::make_pair(key, value);
        }
        this->operations_count++;
        this->writeOperations(table_type, "put", out);
    }
    bool contains(K key) const override {
        int found_index = find_key_index(key);
        this->writeOperations(table_type, "contains", out);
        return found_index != -1;
    }
    V get(K key) const override {
        size_t idx = find_key_index(key);
        if (idx == -1) {
            throw std::runtime_error("Table doesn't contain this key");
        }
        this->writeOperations(table_type, "get", out);
        return table[idx].second;
    }
    void extract(K key) override {
        size_t idx = find_key_index(key);
        if (idx == -1) {
            throw std::runtime_error("Table doesn't contain this key");
        }
        erase_from_vector(idx);
        this->writeOperations(table_type, "extract",out);
    }
    void erase_from_vector(size_t idx) { // to calculate the number of operations
        if (idx >= table.size()) {
            throw std::out_of_range("Index out of range");
        }
        for (size_t i = idx; i + 1 < table.size(); ++i) {
            std::swap(table[i], table[i + 1]); // push the element to the end
            this->operations_count++;
        }
        table.pop_back();
    }
 protected:
    virtual size_t find_key_index(K key) const = 0;
    std::vector<std::pair<K, V>> table;
    std::ostream& out = std::cout; // log stream
    std::string table_type;
};


template<typename K, typename V>
class UnsortedTable : public VectorTable<K, V> {
 public:
    UnsortedTable() : VectorTable<K, V>("UnsortedTable") {}
    explicit UnsortedTable(std::ostream& out) : VectorTable<K, V>(out, "UnsortedTable") {}
 protected:
    size_t find_key_index(K key) const override { // -1 if not found
        for (size_t i = 0; i < this->table.size(); ++i) {
            if (this->equal(this->table[i].first, key)) {
                return i;
            }
        }
        return -1;
    }
};

template<typename K, typename V>
class SortedTable : public VectorTable<K, V> {
 public:
    SortedTable() : VectorTable<K, V>("SortedTable") {}
    explicit SortedTable(std::ostream& out) : VectorTable<K, V>(out, "SortedTable") {}
    void put(K key, V value) override {
        size_t idx = find_key_index(key);
        if (idx != -1) {
            this->table[idx] = std::make_pair(key, value);
            this->operations_count++;
            this->writeOperations("SortedTable", "put", std::cout);
            return;
        }
        this->table.emplace_back(key, value);
        this->operations_count++;
        for (size_t i = this->table.size() - 1; i > 0; --i) {
            if (this->table[i].first < this->table[i - 1].first) {
                std::swap(this->table[i], this->table[i - 1]);
                this->operations_count++;
            } else {
                break;
            }
        }
        this->writeOperations("SortedTable", "put", std::cout);
    }
 protected:
    // not needed after making custom lowerbound
//    struct comparator {
//        bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const {
//            return a.first < b.first;
//        }
//        bool operator()(const std::pair<K, V>& a, const K& b) const { // Чтобы искать только по ключу
//            return a.first < b;
//        }
//        bool operator()(const K& a, const std::pair<K, V>& b) const {
//            return a.first < b;
//        }
//    };

    // to calculate number of operations (unable with lower_bound)
    size_t custom_lower_bound(K key) const {
        int l = -1, r = this->table.size(); // for -1 (size_t is only zero or positive)
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (this->greaterOrEqual(this->table[m].first, key)) {
                r = m;
            } else {
                l = m;
            }
        }
        return r;
    }

    size_t find_key_index(K key) const override { // -1 if not found
        size_t idx = custom_lower_bound(key);

        if (idx == this->table.size()) {
            return -1;
        }
        if (this->equal(this->table[idx].first, key)) {
            return idx;
        }
        return -1;
    }
};
