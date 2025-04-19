#pragma once
#include <iostream>
template<typename K, typename V>
class TableInterface {
 public:
    TableInterface() = default;
    virtual void put(K key, V value) = 0;
    virtual bool contains(K key) const = 0;
    virtual V get(K key) const = 0;
    virtual void extract(K key) = 0;
    virtual void writeOperations(const std::string& type, const std::string& op, std::ostream& out) const {
        out << type << ": " << op << " " << operations_count << std::endl;
        reset_counter();
    }
    virtual void reset_counter() const {
        operations_count = 0;
    }
    virtual bool less(K left_key, K right_key) {
        operations_count++;
        return left_key < right_key;
    }
    virtual bool lessOrEqual(K left_key, K right_key) const {
        operations_count++;
        return left_key <= right_key;
    }
    virtual bool greater(K left_key, K right_key) const {
        operations_count++;
        return left_key > right_key;
    }
    virtual bool greaterOrEqual(K left_key, K right_key) const {
        operations_count++;
        return left_key >= right_key;
    }
    virtual bool equal(K left_key, K right_key) const {
        operations_count++;
        return left_key == right_key;
    }
 protected: // чтобы классы-наследники тоже могли использовать его
    mutable size_t operations_count = 0; // для подсчета операций, но чтобы изменения работали с const методами
};
