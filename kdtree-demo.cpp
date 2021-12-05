#include <SFML/Graphics.hpp>

#include "kd-tree.hpp"
#include "vector.hpp"

template <>
struct Position<Vector> {
    static float getX(Vector const &p) { return p.x; }
    static float getY(Vector const &p) { return p.y; }
};

struct Bounds {
    double xmin, xmax, ymin, ymax;
};

void drawTree(Node<Vector> *node, Bounds bounds, sf::RenderWindow &window,
              sf::VertexArray &lines)
{
    if (node == NULL) {
        return;
    }

    int radius = 5;
    sf::CircleShape circle(radius);
    circle.setPosition(node->getX() - radius, node->getY() - radius);
    circle.setFillColor(sf::Color::Red);
    window.draw(circle);

    // sf::Font font;
    // font.loadFromFile("assets/consola.ttf");
    // sf::Text text((std::string)node->element, font, 15);
    // text.setFillColor(sf::Color(150, 150, 150));
    // text.setPosition(node->getX() + radius + 10, node->getY());
    // window.draw(text);

    sf::Vertex a, b;
    if (node->dim % 2 == 0) {
        a.position = sf::Vector2f(node->getX(), bounds.ymin);
        b.position = sf::Vector2f(node->getX(), bounds.ymax);
        a.color = b.color = sf::Color(200, 50, 30, 100);
    } else {
        a.position = sf::Vector2f(bounds.xmin, node->getY());
        b.position = sf::Vector2f(bounds.xmax, node->getY());
        a.color = b.color = sf::Color(60, 200, 40, 100);
    }
    lines.append(a);
    lines.append(b);

    {
        auto b = bounds;
        if (node->dim % 2 == 0)
            b.xmax = node->getX();
        else
            b.ymax = node->getY();
        drawTree(node->left, b, window, lines);
    }
    {
        auto b = bounds;
        if (node->dim % 2 == 0)
            b.xmin = node->getX();
        else
            b.ymin = node->getY();

        drawTree(node->right, b, window, lines);
    }
}

void drawTree(KDTree<Vector> &kd, sf::RenderWindow &window)
{
    Bounds bounds(
        {0, (double)window.getSize().x, 0, (double)window.getSize().y});
    sf::VertexArray lines(sf::Lines);
    drawTree(kd.getRoot(), bounds, window, lines);
    window.draw(lines);
}

int main()
{
    KDTree<Vector> kd;

    kd.traverse(
        [](Node<Vector> *node) { std::cout << node->element << std::endl; });

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::Vector2i searchPosition;
    sf::Color background(40, 44, 52);
    window.setFramerateLimit(60);
    window.clear(background);

    sf::Font font;
    font.loadFromFile("assets/collegiate.ttf");
    {
        sf::Text text("KD Tree Demonstration", font, 60);
        text.setPosition(
            window.getSize().x / 2 - text.getGlobalBounds().width / 2,
            window.getSize().y / 2 - 2 * text.getGlobalBounds().height / 2);
        text.setFillColor(sf::Color(150, 150, 150));
        window.draw(text);
    }
    {
        sf::Text text("Click anywhere to add an element", font, 30);
        text.setPosition(
            window.getSize().x / 2 - text.getGlobalBounds().width / 2,
            window.getSize().y / 2 - text.getGlobalBounds().height / 2 + 100);
        text.setFillColor(sf::Color(150, 150, 150));
        window.draw(text);
    }
    window.display();
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
        window.clear(background);
        if (searchPosition.x == 0 && searchPosition.y == 0) {
            drawTree(kd, window);
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
        drawTree(kd, window);

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