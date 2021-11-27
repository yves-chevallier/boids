#include "flock.hpp"
#include "boid.hpp"
#include "vector.hpp"

#include <SFML/Graphics.hpp>
#include <deque>
#include <functional>
#include <math.h>

Boid::Boid(Flock &flock, int x, int y,  bool isPredator) :
    flock(flock), 
    position(x, y),
    isPredator(isPredator),
    shape(getShape())
{
    position = Vector::random();
    velocity = Vector::random(flock.maxVelocity * 2.0, -flock.maxVelocity);
}

sf::ConvexShape Boid::getShape() {
    float size = 1;

    int length = 10 * size;
    int width = length / 3;
    sf::ConvexShape shape;
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0, 0));
    shape.setPoint(1, sf::Vector2f(-width * size, length * size));
    shape.setPoint(2, sf::Vector2f(width * size, length * size));
    shape.setOrigin(0, length / 2 * size);
    shape.setFillColor(sf::Color(49, 102, 156, 150));
    shape.setOutlineColor(sf::Color(125, 164, 202, 150));
    shape.setOutlineThickness(length / 10 * size);

    return shape;
}

void Boid::draw(sf::RenderWindow &window) {
    auto windowSize = window.getSize();
    shape.setPosition(sf::Vector2f(position.x * windowSize.x, position.y * windowSize.y));
    shape.setRotation(velocity.angle() * 180 / M_PI + 90);
    window.draw(shape);
}

/**
 * A boid tends to fly toward the center of a group of individuals.
 */
void Boid::cohesion(float radius, float weight)
{
    // Center of the group
    Vector center{0, 0};
    int neighbors = inSight([&](Boid other) { 
        center += other.position; 
    }, radius);
    center /= neighbors;

    // Stir to the center
    velocity += neighbors > 0 ? (center - position) * weight : Vector(0, 0);
}

/**
 * Maintain a certain distance within each others.
 */
void Boid::separation(float separationRadius, float separationStrength)
{
    Vector m{0, 0};

    inSight([&](Boid other) { 
        m += position - other.position; 
    }, flock.separationRadius);

    velocity += m * flock.separation;
}

/**
 * A boid aligns itself with the swarm's average direction.
 */
void Boid::alignment(float alignmentRadius, float alignmentStrength)
{
    Vector sum;

    int neighbors = inSight(
        [&](Boid other) {
            sum += other.velocity;
            neighbors++;
        },
        alignmentRadius);

    velocity += neighbors > 0 ? (sum / neighbors - velocity) * alignmentStrength : Vector(0, 0);
}

/**
 * Move away from nearby predators.
 */
void Boid::fear(float radius, float weight) {
    auto pos = Vector(0, 0);
    pos /= inSight([&](Boid other) { 
        pos += other.isPredator ? other.position : Vector(0, 0);
    }, radius);

    pos -= position;
    pos.normalize();

    position.rotate(pos.angle() * weight);
}

/**
 * The boid would bounce on the edge of the map with
 * a turn factor, at a certain distance (margin) of
 * the edge.
 */
void Boid::bounce(float margin, float turnFactor)
{
    if (position.x < margin) velocity.x += turnFactor;
    if (position.y < margin) velocity.y += turnFactor;
    if (position.x > 1.0 - margin) velocity.x -= turnFactor;
    if (position.y > 1.0 - margin) velocity.y -= turnFactor;
}

/**
 * The boid suddently appear at the opposite of the map
 * if it crosses the boundaries.
 */
void Boid::wrap()
{
    if (position.x < 0) position.x += 1.0;
    if (position.y < 0) position.y += 1.0;

    if (position.x > 1.0) position.x -= 1.0;
    if (position.y > 1.0) position.y -= 1.0;
}

void Boid::compute()
{
    cohesion(flock.cohesionRadius, flock.cohesion);
    separation(flock.separationRadius, flock.separation);
    alignment(flock.alignmentRadius, flock.alignment);
    // fear(flock.fearRadius, flock.fear);

    if (flock.wrap)
        wrap();
    else
        bounce(speed() * 5.0, speed() / 5.0);

    velocity.limit(flock.maxVelocity);
    position += velocity;
}

int Boid::inSight(std::function<const void(Boid &boid)> callback, float radius)
{
    float amin = velocity.angle() - flock.fieldOfView / 2;
    float amax = velocity.angle() + flock.fieldOfView / 2;

    int neighbors = 0;
    flock.each([&](Boid &other) {
        float a = angleTo(other);
        if (distanceTo(other) < radius && a > amin && a < amax) {
            callback(other);
            neighbors++;
        }
    });
    return neighbors;
}

float Boid::speed()
{
    return velocity.norm();
}

float Boid::angle() { return velocity.angle(); }
float Boid::angleTo(Boid &other) { return position.angle(other.position); }

float Boid::distanceTo(Boid &other, bool wrap)
{
    return wrap ? position.toroidal_distance(other.position)
                : position.distance(other.position);
}
