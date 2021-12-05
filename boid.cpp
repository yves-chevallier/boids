#include "flock.hpp"
#include "boid.hpp"
#include "vector.hpp"

#include <SFML/Graphics.hpp>
#include <deque>
#include <functional>
#include <math.h>

Boid::Boid(Flock &flock, const Vector &position, bool isPredator=false) :
    flock(flock), 
    isPredator(isPredator)
{
    this->position = position;
    velocity = Vector::random(flock.maxVelocity * 2.0, -flock.maxVelocity);
}

Boid::Boid(Flock &flock, int x, int y, bool isPredator) :
    Boid(flock, Vector(x, y), isPredator)
{
}

Boid::Boid(Flock &flock, bool isPredator) :
    Boid(flock, Vector::random(), isPredator)
{
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

void Boid::update()
{
    // cohesion(flock.cohesionRadius, flock.cohesion);
    // separation(flock.separationRadius, flock.separation);
    // alignment(flock.alignmentRadius, flock.alignment);
    // fear(flock.fearRadius, flock.fear);

    Mobile::update();
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

