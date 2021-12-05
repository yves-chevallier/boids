#include "scene.hpp"
#include "flock.hpp"

#include <SFML/Graphics.hpp>

Scene::Scene(sf::RenderWindow &window, Flock &flock) : window(window), shape(sf::Triangles), tails(sf::Lines)
{
}

void Scene::add(sf::VertexArray &array, float x, float y, float angle)
{
    int boidWidth = 3;
    int boidHeight = 10;

    // set the center position
    sf::Vertex v1(sf::Vector2f(x, y));
    sf::Vertex v2(
        sf::Vector2f(v1.position.x - boidWidth, v1.position.y - boidHeight));
    sf::Vertex v3(
        sf::Vector2f(v1.position.x + boidWidth, v1.position.y - boidHeight));

    // setting color
    v1.color = v2.color = v3.color = sf::Color(150, 120, 156, 150);

    // set the angle
    sf::Transform transform;
    transform.rotate(angle, (v2.position.x + v3.position.x) / 2,
                     v1.position.y - 5);
    v1.position = transform.transformPoint(v1.position);
    v2.position = transform.transformPoint(v2.position);
    v3.position = transform.transformPoint(v3.position);

    // appending them into vertex array
    array.append(v1);
    array.append(v2);
    array.append(v3);
}

void Scene::draw(sf::RenderTarget &target, sf::RenderStates states) const { 
    sf::VertexArray array(sf::Triangles, flock.boids.size() * 3);
    for (auto &boid : flock.boids) {
        add(array, boid.position.x, boid.position.y, boid.angle);
    }
    window.draw(array); 
}
