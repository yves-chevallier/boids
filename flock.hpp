/**
 * Collection of boids (swarm).
 */
#pragma once

#include <functional>
#include <vector>
#include <cmath>

#include "boid.hpp"

class Flock
{
   public:
    /**
     * Each boid flies towards the the other boids. But they don't just
     * immediately fly directly at each other. They gradually steer towards
     * each other at a rate that you can adjust with the "cohesion" slider.
     */
    double cohesion = 0.05;
    double cohesionRadius = 0.075;

    /**
     * Each boid also tries to avoid running into the other boids. If it
     * gets too close to another boid it will steer away from it. You can
     * control how quickly it steers with the "separation" slider.
     */
    double separation = 0.05;
    double separationRadius = 0.025;

    /**
     * Eventually, each boid tries to match the vector (speed and direction)
     * of the other boids around it. Again, you can control how quickly
     * they try to match vectors using the "alignment" slider.
     */
    double alignment = 0.05;
    double alignmentRadius = 0.050;

    /**
     * Fear
     */
    double fear = 0.05;
    double fearRadius = 0.075;

    /**
     * Angle of vision of each boid. A 360° range meant the boid can see
     * anywhere around itself.
     */
    double fieldOfView = 210 * (2.0 * (atan(1.0) * 4.0) / 360.0);

    /**
     * Maximum speed for boids in this very universe.
     */
    double maxVelocity = 0.001;

    /**
     * How the boids would behave at the boundaries.
     */
    bool wrap = false;

    int tailLength = 20;

    Flock(unsigned numBoids = 100);

    void compute();
    void draw(sf::RenderWindow &window);

    void add();
    void add(double x, double y);
    void resize(unsigned size);

    void each(std::function<const void(Boid &boid)> callback);

    unsigned size();

   private:
    std::vector<Boid> boids;

    void init(unsigned size);
};
