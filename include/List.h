#include <stdexcept>

template<typename T>
class List {
 private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val = T(), Node* nxt = nullptr) : data(val), next(nxt) {}
    };

    Node* head;
    Node* tail;
    size_t len = 0;

 public:
    List() : head(new Node()), tail(head) {}
    // rule of three
    ~List() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    List(const List& other) : head(new Node()), len(other.len){
        Node* current = head;
        Node* other_current = other.head->next;
        while (other_current != nullptr) {
            current->next = new Node(other_current->data);
            tail = current;
            current = current->next;
            other_current = other_current->next;
        }
    }
    List& operator=(const List& other) {
        if (this != &other) {
            Node* current = head->next;
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }

            Node* currentThis = head;
            tail = head;
            len = other.len;
            Node* otherCurrent = other.head->next;

            while (otherCurrent) {
                currentThis->next = new Node(otherCurrent->data);
                tail = currentThis;
                currentThis = currentThis->next;
                otherCurrent = otherCurrent->next;
            }
        }
        return *this;
    }
    bool empty() const {
        return head->next == nullptr;
    }
    size_t size() const {
        return len;
    }
    List& push_front(const T& value) {
        Node* current = new Node(value);
        current->next = head->next;
        if (current->next == nullptr) {
            tail = current;
        }
        head->next = current;
        len++;
        return *this;
    }
    List& push_back(const T& value) {
        tail->next = new Node(value);
        tail = tail->next;
        len++;
        return *this;
    }
    //TODO TESTS
    T& get_first() const {
        if (head->next == nullptr) {
            throw std::runtime_error("Cannot extract from an empty list.");
        }
        T& result = head->next->data;
        return result;
    }
    // TODO TESTS
    T& back() const {
        if (tail == head) {
            throw std::runtime_error("The list is empty.");
        }
        return tail->data;
    }
    T extract() {
        if (head->next == nullptr) {
            throw std::runtime_error("Cannot extract from an empty list.");
        }
        Node* node = head->next;
        T result = node->data;
        head->next = node->next;
        if (node->next == nullptr) {
            tail = head;
        }
        delete node;
        --len;
        return result;
    }
};

template<typename T>
List<T> merge_sorted_lists(List<T> a, List<T> b, bool ascending = true) {
    List<T> result;
    while (!a.empty() && !b.empty()) {
        if (ascending) {
            if ((a.get_first() < b.get_first())) {
                result.push_back(a.extract());
            } else {
                result.push_back(b.extract());
            }
        } else {
            if ((a.get_first() > b.get_first())) {
                result.push_back(a.extract());
            } else {
                result.push_back(b.extract());
            }
        }
    }
    while (!a.empty()) {
        result.push_back(a.extract());
    }
    while (!b.empty()) {
        result.push_back(b.extract());
    }
    return result;
}

template<typename T>
List<T>& merge_sort(List<T>& list, bool ascending = true) {
    if (list.size() <= 1) {
        return list;
    }
    List<T> first_half;
    while (first_half.size() < list.size()) {
        first_half.push_back(list.extract());
    }
    merge_sort(first_half, ascending);
    merge_sort(list, ascending);
    list = merge_sorted_lists(first_half, list, ascending);
    return list;
}