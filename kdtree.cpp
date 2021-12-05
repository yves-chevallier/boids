#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <stack>
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
    int dim;
    Node(T element, int dim = 0)
        : element(element), left(NULL), right(NULL), dim(dim)
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

        if (node == NULL) {
            root = new Node<T>(element);
            return;
        }

        if (node->element == element) {
            return;
        }

        bool comp = node->dim % 2 == 0 ? x < node->getX() : y < node->getY();
        Node<T> **indirect = comp ? &node->left : &node->right;
        if (*indirect == NULL) {
            *indirect = new Node<T>(element, node->dim + 1);
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

        // std::cout << "Exploring node " << node->element << std::endl;

        if (node->dim % 2 == 0 ? x - r < node->getX() : y - r < node->getY()) {
            searchNode(node->left, element, r, ids);
        }
        if (node->dim % 2 == 0 ? x + r > node->getX() : y + r > node->getY()) {
            searchNode(node->right, element, r, ids);
        }
        double dist = (x - node->getX()) * (x - node->getX()) +
                      (y - node->getY()) * (y - node->getY());
        if (dist < r * r) {
            ids.push_back(node->element);
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
        func(node);
        if (node->left != NULL) {
            traverseNode(node->left, func);
        }
        if (node->right != NULL) {
            traverseNode(node->right, func);
        }
    }

    void print(const std::string &prefix, const Node<T> *node, bool isLeft)
    {
        if (node == NULL) return;
        std::cout << prefix << (isLeft ? "├──" : "└──") << node->element
                  << std::endl;
        print(prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }

   public:
    KDTree() : root(NULL) {}
    void remove(int id) { removeNode(root, id); }

    void print() { print("", root, false); }

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
            points.push_back(node->element);
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
                node = NULL;
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

void drawTree(Node<Vector> *node, int min, int max, sf::RenderWindow &window)
{
    if (node == NULL) {
        return;
    }

    std::cout << node->element << " : min(" << min << ") max(" << max << ")"
              << std::endl;

    int radius = 5;
    sf::CircleShape circle(radius);
    circle.setPosition(node->getX() - radius, node->getY() - radius);
    circle.setFillColor(sf::Color::Red);
    window.draw(circle);

    if (node->dim % 2 == 0) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(node->getX(), min)),
            sf::Vertex(sf::Vector2f(node->getX(), max)),
        };
        sf::Color color(200, 50, 30, 100);
        line[0].color = color;
        line[1].color = color;
        window.draw(line, 2, sf::Lines);
    } else {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(min, node->getY())),
            sf::Vertex(sf::Vector2f(max, node->getY())),
        };
        sf::Color color(60, 200, 40, 100);
        line[0].color = color;
        line[1].color = color;

        window.draw(line, 2, sf::Lines);
    }

    drawTree(node->left, min, node->dim % 2 == 0 ? node->getX() : node->getY(),
             window);
    drawTree(node->right, node->dim % 2 == 0 ? node->getX() : node->getY(), max,
             window);
}
int main()
{
    KDTree<Vector> kd;

    kd.traverse(
        [](Node<Vector> *node) { std::cout << node->element << std::endl; });

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::Vector2i searchPosition;
    window.setFramerateLimit(5);
    while (window.isOpen()) {
        sf::Event event;

        bool redraw = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mouse = sf::Mouse::getPosition(window);
                kd.insert({(double)mouse.x, (double)mouse.y});
                // kd.print();
                redraw = true;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                searchPosition = sf::Mouse::getPosition(window);
                redraw = true;
            }
        }

        if (!redraw) {
            continue;
        }
        window.clear();
        if (searchPosition.x == 0 && searchPosition.y == 0) {
            drawTree(kd.getRoot(), 0, 1000, window);

            window.display();
            continue;
        }

        int radius = 150.0f;
        sf::CircleShape search(radius);
        search.setPosition(searchPosition.x - radius,
                           searchPosition.y - radius);
        auto col = sf::Color(100, 90, 200, 90);
        search.setFillColor(col);
        window.draw(search);
        drawTree(kd.getRoot(), 0, 1000, window);

        for (auto &e :
             kd.search({(double)searchPosition.x, (double)searchPosition.y},
                       radius)) {
            int radius = 5;
            sf::CircleShape shape(radius);
            auto col = sf::Color(100, 90, 200, 255);
            shape.setFillColor(col);
            shape.setPosition(e.x - radius, e.y - radius);
            window.draw(shape);
        }

        window.display();
    }
}