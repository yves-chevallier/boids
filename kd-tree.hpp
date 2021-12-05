#pragma once
#include <functional>
#include <iostream>
#include <stack>
#include <vector>

template <class Geometry>
struct Position;

// template <>
// struct Position<Vector> {
//     static float getX(Vector const &p) { return p.x; }
//     static float getY(Vector const &p) { return p.y; }
// };

template <typename T>
struct Node {
    T element;
    Node<T> *left, *right;
    int dim;
    Node(T element, int dim = 0)
        : element(element), left(nullptr), right(nullptr), dim(dim)
    {
    }

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

        if (node == nullptr) {
            root = new Node<T>(element);
            return;
        }

        if (node->element == element) {
            return;
        }

        bool comp = node->dim % 2 == 0 ? x < node->getX() : y < node->getY();
        Node<T> **indirect = comp ? &node->left : &node->right;
        if (*indirect == nullptr) {
            *indirect = new Node<T>(element, node->dim + 1);
            return;
        }
        insertNode(*indirect, element);
    }

    void searchNode(Node<T> *node, T element, double r, std::vector<T> &ids)
    {
        double x = Position<T>::getX(element);
        double y = Position<T>::getY(element);

        if (node == nullptr) {
            return;
        }

        if (node->dim % 2 == 0 ? x - r < node->getX() : y - r < node->getY()) 
            searchNode(node->left, element, r, ids);
        
        if (node->dim % 2 == 0 ? x + r > node->getX() : y + r > node->getY()) 
            searchNode(node->right, element, r, ids);
        
        double dist = (x - node->getX()) * (x - node->getX()) +
                      (y - node->getY()) * (y - node->getY());

        if (dist < r * r) 
            ids.push_back(node->element);
        
    }
    void removeNode(Node<T> *node, int id)
    {
        if (node == nullptr) {
            return;
        }

        if (node->id == id) {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return;
            }
            if (node->left == nullptr) {
                Node<T> *temp = node->right;
                delete node;
                node = temp;
                return;
            }
            if (node->right == nullptr) {
                Node<T> *temp = node->left;
                delete node;
                node = temp;
                return;
            }
            Node<T> *temp = node->right;
            while (temp->left != nullptr) {
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
        if (node == nullptr) return;
    }

    void traverseNode(Node<T> *node, std::function<void(Node<T> *)> func)
    {
        if (node == nullptr) {
            return;
        }
        func(node);
        if (node->left != nullptr) {
            traverseNode(node->left, func);
        }
        if (node->right != nullptr) {
            traverseNode(node->right, func);
        }
    }

    void print(const std::string &prefix, const Node<T> *node, bool isLeft)
    {
        if (node == nullptr) return;
        std::cout << prefix << (isLeft ? "├◐─" : "└◑─") << node->element
                  << std::endl;
        print(prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }

   public:
    KDTree() : root(nullptr) {}
    void remove(int id) { removeNode(root, id); }

    void print() { print("", root, false); }

    void insert(T element)
    {
        if (root == nullptr) {
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
        if (root == nullptr) {
            return points;
        }
        std::vector<Node<T> *> nodes;
        nodes.push_back(root);
        while (nodes.size() > 0) {
            Node<T> *node = nodes.back();
            nodes.pop_back();
            points.push_back(node->element);
            if (node->left != nullptr) {
                nodes.push_back(node->left);
            }
            if (node->right != nullptr) {
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
        std::stack<Node<T> *> stack;

        iterator(Node<T> *node) : node(node)
        {
            auto root = node;

            if (root != nullptr) {
                stack.push(node);
                root = root->left;
            }

            if (stack.size() > 0) {
                node = stack.top();
                stack.pop();
            } else {
                node = nullptr;
            }
        }
        iterator &operator++()
        {
            if (node->right) {
                stack.push(node->right);

                if (node->right->left) stack.push(node->right->left);
            }

            if (stack.size() == 0) {
                node = nullptr;
                return *this;
            }

            node = stack.top();
            stack.pop();

            return *this;
        }
        bool operator!=(const iterator &other) { return node != other.node; }
        T operator*() { return node->element; }
    };

    iterator begin() { return iterator(root); }
    iterator end() { return iterator(nullptr); }

    Node<T> *getRoot() { return root; }
};
