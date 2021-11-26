#include "flock.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

class Window {
    int frameRate;
    int width;
    int height;
    float fps;
    sf::RenderWindow window;

    sf::Color backgroundColor;

    sf::Clock dtClock;
    sf::Clock fpsTimer;

public:
    Window(int width, int height, std::string title) : 
        window(sf::VideoMode(width, height), title), 
        width(width), height(height), frameRate(60) {
            backgroundColor = sf::Color(20, 30, 50);
            fps = 0;
        }

    void clear() {
        window.clear(backgroundColor);
    }
    void display() {
        window.display();
    }
    bool isOpen() {
        return window.isOpen();
    }

    void close() {
        window.close();
    }

    void init() {
        
        window.setFramerateLimit(frameRate);

        sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
        sf::View originalView = view;       
    }    

    float computeFps() {
        if (fpsTimer.getElapsedTime().asSeconds() > 1) {
            fps = frameRate;
            fpsTimer.restart();
        }
        return fps;
    }

    void run() {
        Flock flock(10);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            clear();
            flock.compute();
            flock.draw(window);
            display();
        }
    }
};

int main(int argc, char* argv[])
{
    Window window(800, 600, "SFML Boids");
    window.run();
}