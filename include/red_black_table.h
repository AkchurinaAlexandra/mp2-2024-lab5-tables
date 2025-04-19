#include <table_interface.h>

template<typename K, typename V>
class RedBlackTree : public TableInterface<K, V> {
  public:
    RedBlackTree() = default;
    explicit RedBlackTree(std::ostream& out) : out(out) {}
    ~RedBlackTree() {
        clear(root);
    }

    void put(K key, V value) override {
        Node* inserted = nullptr;
        root = insert(root, key, value, nullptr);
        inserted = find(root, key);
        insertFixup(inserted);
        this->writeOperations("RedBlackTree", "put", out);
    }

    bool contains(K key) const override {
        Node* res = find(root, key);
        this->writeOperations("RedBlackTree", "contains", out);
        return res != nullptr;
    }

    V get(K key) const override {
        Node* node = find(root, key);
        this->writeOperations("RedBlackTree", "get", out);
        if (!node) {
            throw std::runtime_error("Key not found.");
        }
        return node->value;
    }

    void extract(K key) override {
        Node* node = find(root, key);
        if (!node) {
            throw std::runtime_error("Key not found.");
        }
        deleteNode(node);
        this->writeOperations("RedBlackTree", "extract", out);
    }
 private:
    enum Color { RED, BLACK };

    struct Node {
        K key;
        V value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(K k, V v, Color c, Node* p = nullptr)
            : key(k), value(v), color(c), left(nullptr), right(nullptr), parent(p) {}
    };

    Node* root{nullptr};
    std::ostream& out{std::cout};

    void leftRotate(Node* x) {
        this->operations_count++;

        Node* y = x->right;
        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        this->operations_count++;

        Node* y = x->left;
        x->left = y->right;

        if (y->right != nullptr)
            y->right->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node* z) {
        this->operations_count++;
        while (z->parent && z->parent->color == RED) {
            Node* gp = z->parent->parent;
            if (z->parent == gp->left) {
                Node* y = gp->right;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rightRotate(gp);
                }
            } else {
                Node* y = gp->left;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    leftRotate(gp);
                }
            }
        }
        root->color = BLACK;
    }

    Node* insert(Node* node, K key, V value, Node* parent) {
        this->operations_count++;
        if (node == nullptr) {
            return new Node(key, value, RED, parent);
        }
        if (key < node->key) {
            node->left = insert(node->left, key, value, node);
        } else if (key > node->key) {
            node->right = insert(node->right, key, value, node);
        } else {
            node->value = value;
        }

        return node;
    }

    Node* find(Node* node, K key) const {
        this->operations_count++;
        while (node != nullptr) {
            this->operations_count++;
            if (key < node->key) {
                node = node->left;
            }else if (key > node->key) {
                node = node->right;
            } else {
                return node;
            }
        }
        return nullptr;
    }

    Node* minimum(Node* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void replaceSubtree(Node* u, Node* v) {
        this->operations_count++;
        if (u->parent == nullptr) {
            root = v;
        }else if (u == u->parent->left) {
            u->parent->left = v;
        }else {
            u->parent->right = v;
        }if (v) {
            v->parent = u->parent;
        }
    }

    void fixForDelete(Node* x, Node* x_parent) {
        this->operations_count++;
        while (x != root && (!x || x->color == BLACK)) {
            if (x == x_parent->left) {
                Node* w = x_parent->right;
                if (w && w->color == RED) {
                    w->color = BLACK;
                    x_parent->color = RED;
                    leftRotate(x_parent);
                    w = x_parent->right;
                }
                if ((w == nullptr || (w->left == nullptr || w->left->color == BLACK) && (w->right == nullptr || w->right->color == BLACK))) {
                    if (w) {
                        w->color = RED;
                    }
                    x = x_parent;
                    x_parent = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left) {
                            w->left->color = BLACK;
                        }
                        if (w) {
                            w->color = RED;
                        }
                        rightRotate(w);
                        w = x_parent->right;
                    }
                    if (w != nullptr) {
                        w->color = x_parent->color;
                    }
                    x_parent->color = BLACK;
                    if (w && w->right) {
                        w->right->color = BLACK;
                    }
                    leftRotate(x_parent);
                    x = root;
                }
            } else {
                Node* w = x_parent->left;
                if (w != nullptr && w->color == RED) {
                    w->color = BLACK;
                    x_parent->color = RED;
                    rightRotate(x_parent);
                    w = x_parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (!w->left || w->left->color == BLACK)) {
                    if (w) {
                        w->color = RED;
                    }
                    x = x_parent;
                    x_parent = x->parent;
                } else {
                    if (!w->left || w->left->color == BLACK) {
                        if (w->right) {
                            w->right->color = BLACK;
                        }
                        if (w) {
                            w->color = RED;
                        }
                        leftRotate(w);
                        w = x_parent->left;
                    }
                    if (w) {
                        w->color = x_parent->color;
                    }
                    x_parent->color = BLACK;
                    if (w && w->left) {
                        w->left->color = BLACK;
                    }
                    rightRotate(x_parent);
                    x = root;
                }
            }
        }
        if (x) x->color = BLACK;
    }

    void deleteNode(Node* node) {
        Node* y = node;
        Node* x = nullptr;
        Node* x_parent = nullptr;
        Color y_original_color = y->color;

        if (node->left == nullptr) {
            x = node->right;
            x_parent = node->parent;
            replaceSubtree(node, node->right);
        } else if (node->right == nullptr) {
            x = node->left;
            x_parent = node->parent;
            replaceSubtree(node, node->left);
        } else {
            y = minimum(node->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == node) {
                if (x) {
                    x->parent = y;
                }
                x_parent = y;
            } else {
                replaceSubtree(y, y->right);
                y->right = node->right;
                if (y->right) {
                    y->right->parent = y;
                }
                x_parent = y->parent;
            }
            replaceSubtree(node, y);
            y->left = node->left;
            if (y->left) {
                y->left->parent = y;
            }
            y->color = node->color;
        }
        delete node;
        if (y_original_color == BLACK) {
            fixForDelete(x, x_parent);
        }
    }

    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }
};
