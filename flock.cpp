/**
 * Collection of boids (swarm).
 */
#include "flock.hpp"
#include "boid.hpp"

Flock::Flock(unsigned size) { init(size); }

void Flock::init(unsigned size)
{
    boids.clear();
    for (unsigned i = 0; i < size; i++) boids.push_back(Boid(*this));
}

void Flock::resize(unsigned size)
{
    while (boids.size() > size) boids.pop_back();
    while (boids.size() < size) boids.push_back(Boid(*this));
}

void Flock::each(std::function<const void(Boid &boid)> callback)
{
    for (auto &boid : boids) callback(boid);
}

void Flock::compute()
{
    for (auto &boid : boids) {
        boid.compute();
    }
}

void Flock::add() {
    boids.push_back(Boid(*this));
}

void  Flock::add(double x, double y) {
    boids.push_back(Boid(*this, x, y));
}

unsigned Flock::size() { return boids.size(); }

