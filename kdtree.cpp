#include <vector>
#include <iostream>
#include <functional>

using Vector = std::pair<double, double>;

struct Node {
    int id;
    Vector point;
    Node *left, *right;
    Node(int id, double x, double y) : id(id), point(x, y), left(NULL), right(NULL) {}
};

class KDTree {
    Node *root;
    void insertNode(Node *node, int id, double x, double y);
    void searchNode(Node *node, double x, double y, double r, std::vector<int> &ids);
    void removeNode(Node *node, int id);
    void clearNode(Node *node);
    void traverseNode(Node *node, std::function<void(Node*)> func);
public:
    KDTree() : root(NULL) {}
    void remove(int id);
    void insert(int id, double x, double y);
    std::vector<int> search(double x, double y, double r);
    void clear();

    std::vector<Vector> traverse();
    void traverse(std::function<void(Node*)> func);

    class iterator : public std::iterator<std::input_iterator_tag, Node> {
        public:
            Node *node;
            iterator(Node *node) : node(node) {}
            iterator& operator++() {
                node = node->right;
                while (node->left) node = node->left;
                return *this;
            }
            bool operator!=(const iterator& other) {
                return node != other.node;
            }
            Vector operator*() {
                return node->point;
            }
        };

    iterator begin() {
        Node *node = root;
        while (node->left) node = node->left;
        return iterator(node);
    }
    iterator end() {
        return iterator(NULL);
    }
};

void KDTree::traverse(std::function<void(Node*)> func) {
    traverseNode(root, func);
}

void KDTree::traverseNode(Node *node, std::function<void(Node*)> func) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        func(node->left);
        traverseNode(node->left, func);
    }
    if (node->right != NULL) {
        func(node->right);
        traverseNode(node->left, func);
    }
}

std::vector<Vector> KDTree::traverse() {
    std::vector<Vector> points;
    if (root == NULL) {
        return points;
    }
    std::vector<Node *> nodes;
    nodes.push_back(root);
    while (nodes.size() > 0) {
        Node *node = nodes.back();
        nodes.pop_back();
        points.push_back(node->point);
        if (node->left != NULL) {
            nodes.push_back(node->left);
        }
        if (node->right != NULL) {
            nodes.push_back(node->right);
        }
    }
    return points;
}

void KDTree::clear() {
    clearNode(root);
}

void KDTree::clearNode(Node *node) {
    if (node == NULL) return;
}

std::vector<int> KDTree::search(double x, double y, double r) {
    std::vector<int> ids;
    searchNode(root, x, y, r, ids);
    return ids;
}

void KDTree::insert(int id, double x, double y) {
    if (root == NULL) {
        root = new Node(id, x, y);
        return;
    }
    insertNode(root, id, x, y);
}

void KDTree::remove(int id) {
    removeNode(root, id);
}

void KDTree::insertNode(Node *node, int id, double x, double y) {
    if (node == NULL) {
        root = new Node(id, x, y);
        return;
    }
    if (node->point.first == x && node->point.second == y) {
        return;
    }
    Node **indirect = node->point.first < x ? &node->right : &node->left;
    if (*indirect == NULL) {
        *indirect = new Node(id, x, y);
        return;
    }
    insertNode(*indirect, id, x, y);
}

void KDTree::searchNode(Node *node, double x, double y, double r, std::vector<int> &ids) {
    if (node == NULL) {
        return;
    }
    if (node->point.first < x) {
        searchNode(node->right, x, y, r, ids);
    }
    if (node->point.first - r <= x && node->point.second - r <= y && node->point.first + r >= x && node->point.second + r >= y) {
        ids.push_back(node->id);
    }
    if (node->point.first > x) {
        searchNode(node->left, x, y, r, ids);
    }
}

void KDTree::removeNode(Node *node, int id) {
    if (node == NULL) {
        return;
    }
    if (node->id == id) {
        if (node->left == NULL && node->right == NULL) {
            delete node;
            return;
        }
        if (node->left == NULL) {
            Node *temp = node->right;
            delete node;
            node = temp;
            return;
        }
        if (node->right == NULL) {
            Node *temp = node->left;
            delete node;
            node = temp;
            return;
        }
        Node *temp = node->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        node->id = temp->id;
        removeNode(node->right, temp->id);
        return;
    }
    removeNode(node->point.first < id ? node->right : node->left, id);
}
