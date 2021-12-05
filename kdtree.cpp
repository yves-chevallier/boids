#include <functional>
#include <iostream>
#include <vector>

#include "vector.hpp"
//#include "mobile.hpp"

template <class Geometry>
struct Position;

// template <>
// struct Position<Mobile>
// {
//     float getX(Mobile const& p) { return p.position.x; }
//     float getY(Mobile const& p) { return p.position.y; }
// };

template <>
struct Position<Vector> {
    static float getX(Vector const &p) { return p.x; }
    static float getY(Vector const &p) { return p.y; }
};

template <typename T>
struct Node {
    T element;
    Node<T> *left, *right;
    Node(T element) : element(element), left(NULL), right(NULL) {}

    double getX() { return Position<T>::getX(element); }
    double getY() { return Position<T>::getY(element); }
};

template <typename T>
class KDTree
{
    Node<T> *root;
    void insertNode(Node<T> *node, T element)
    {
        double x = Position<T>::getX(element);
        double y = Position<T>::getY(element);

        if (node == NULL) {
            root = new Node<T>(element);
            return;
        }

        if (node->element == element) {
            return;
        }
        
        Node<T> **indirect = node->getX() < x ? &node->right : &node->left;
        if (*indirect == NULL) {
            *indirect = new Node<T>(element);
            return;
        }
        insertNode(*indirect, element);
    }

    void searchNode(Node<T> *node, T element, double r, std::vector<T> &ids)
    {
        double x = Position<T>::getX(element);
        double y = Position<T>::getY(element);

        if (node == NULL) {
            return;
        }
        if (node->getX() < x) {
            searchNode(node->right, element, r, ids);
        }
        if (node->getX() - r <= x && node->getY() - r <= y &&
            node->getX() + r >= x && node->getY() + r >= y) {
            ids.push_back(element);
        }
        if (node->getX() > x) {
            searchNode(node->left, element, r, ids);
        }
    }
    void removeNode(Node<T> *node, int id)
    {
        if (node == NULL) {
            return;
        }
        if (node->id == id) {
            if (node->left == NULL && node->right == NULL) {
                delete node;
                return;
            }
            if (node->left == NULL) {
                Node<T> *temp = node->right;
                delete node;
                node = temp;
                return;
            }
            if (node->right == NULL) {
                Node<T> *temp = node->left;
                delete node;
                node = temp;
                return;
            }
            Node<T> *temp = node->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            node->id = temp->id;
            removeNode(node->right, temp->id);
            return;
        }
        removeNode(node->getX() < id ? node->right : node->left, id);
    }

    void clearNode(Node<T> *node)
    {
        if (node == NULL) return;
    }
    void traverseNode(Node<T> *node, std::function<void(Node<T> *)> func)
    {
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
    void print(const std::string& prefix, const Node<T>* node, bool isLeft)
    {
        if (node == NULL) return;
        std::cout << prefix << (isLeft ? "├──" : "└──") << node->element << std::endl;
        print(prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
   public:
    KDTree() : root(NULL) {}
    void remove(int id) { removeNode(root, id); }

    void print()
    {
        print("", root, false);
    }

    void insert(T element)
    {
        if (root == NULL) {
            root = new Node<T>(element);
            return;
        }
        insertNode(root, element);
    }
    std::vector<T> search(T element, double r)
    {
        std::vector<T> ids;
        searchNode(root, element, r, ids);
        return ids;
    }
    void clear() { clearNode(root); }

    std::vector<Vector> traverse()
    {
        std::vector<Vector> points;
        if (root == NULL) {
            return points;
        }
        std::vector<Node<T> *> nodes;
        nodes.push_back(root);
        while (nodes.size() > 0) {
            Node<T> *node = nodes.back();
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

    void traverse(std::function<void(Node<T> *)> func)
    {
        traverseNode(root, func);
    }

    class iterator : public std::iterator<std::input_iterator_tag, Node<T> >
    {
       public:
        Node<T> *node;
        iterator(Node<T> *node) : node(node) {}
        iterator &operator++()
        {
            node = node->right;
            while (node->left) node = node->left;
            return *this;
        }
        bool operator!=(const iterator &other) { return node != other.node; }
        Vector operator*() { return node->point; }
    };

    iterator begin()
    {
        Node<T> *node = root;
        while (node->left) node = node->left;
        return iterator(node);
    }
    iterator end() { return iterator(NULL); }
};

int main()
{
    KDTree<Vector> kd;
    kd.insert({7, 2});
    kd.insert({5, 4});
    kd.insert({9, 6});
    kd.insert({2, 3});
    kd.insert({4, 7});
    kd.insert({8, 1});

    kd.print();

    for (auto k : kd.search(Vector(2, 2), 5)) {
        std::cout << k << std::endl;
    }
}