#include <table_interface.h>
#include <vector>

template<typename K, typename V>
class HashTable : public TableInterface<K, V>{
 public:
    using TableInterface<K, V>::equal;

    const size_t DEFAULT_CAPACITY = 100000;
    explicit HashTable(std::ostream& out = std::cout) : capacity(DEFAULT_CAPACITY), table(DEFAULT_CAPACITY), out(out) {}
    explicit HashTable(size_t capacity, std::ostream& out=std::cout) : capacity(capacity), table(capacity), out(out) {}
    void put(K key, V value) override {
        size_t idx = findExistingKey(key);
        if (idx != -1) {
            table[idx].value = value;
            table[idx].is_deleted = false;
            this->operations_count++;
            this->writeOperations("HashTable", "put", out);
            return;
        }
        if (size >= capacity) {
            throw std::runtime_error("Table is full");
        }
        size_t hash = hashKey(key);
        for (int i = 0; i < capacity; ++i) {
            this->operations_count++;
            size_t index = check_next(hash, i);
            if (table[index].is_empty || table[index].is_deleted) {
                table[index].key = key;
                table[index].value = value;
                table[index].is_empty = false;
                table[index].is_deleted = false;
                size++;
                this->writeOperations("HashTable", "put", out);
                return;
            }
        }
        throw std::runtime_error("Could not insert key and value");
    }
    bool contains(K key) const override {
        int result = findExistingKey(key);
        this->writeOperations("HashTable", "contains", out);
        return (result != -1); // TODO ????
    }
    V get(K key) const override {
        size_t idx = findExistingKey(key);
        if (idx == -1) {
            throw std::runtime_error("Table doesn't contain this key");
        }
        this->operations_count++;
        this->writeOperations("HashTable", "get", out);
        return table[idx].value;
    }
    void extract(K key) override {
        size_t idx = findExistingKey(key);
        if (idx == -1) {
            throw std::runtime_error("Table doesn't contain this key");
        }
        table[idx].is_deleted = true;
        this->operations_count++;
        this->writeOperations("HashTable", "extract", out);
    }

 private:
    struct HashNode {
        K key;
        V value;
        bool is_deleted = false;
        bool is_empty = true;
        HashNode() = default;
    };

    size_t hashKey(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }
    [[nodiscard]] size_t check_next(size_t hash, int i) const {
        return (hash + i) % capacity;
    }
    size_t findExistingKey(K key) const{
        size_t hash = hashKey(key);
        for (int i = 0; i < capacity; ++i) {
            this->operations_count++;
            size_t index = check_next(hash, i);
            if (table[index].is_empty) {
                return -1;
            }
            if (equal(table[index].key, key) && !table[index].is_deleted) {
                return index;
            }
        }
        return -1;
    }

    std::vector<HashNode> table;
    size_t capacity = 0;
    size_t size = 0;
    std::ostream& out;
};