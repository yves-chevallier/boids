#pragma once

#include "flock.hpp"
#include <SFML/Graphics.hpp>
#include <map>

class Scene : public sf::Drawable
{
    int width, height;
    sf::VertexArray shape; // Boid shapes
    sf::VertexArray tails; // Boid tails

    sf::RenderWindow &window;

    Flock &flock;

    void add(sf::VertexArray &array, float x, float y, float angle);
public:
    Scene(sf::RenderWindow &window, Flock &flock);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};