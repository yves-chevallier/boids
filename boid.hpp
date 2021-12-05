#pragma once

#include "flock.hpp"
#include "mobile.hpp"
#include "vector.hpp"

#include <functional>

class Flock;

class Boid : public Mobile {   
    int size;

    bool isPredator;

    Flock &flock; // Friend reference

public:
    Boid(Flock &flock, bool isPredator=false);
    Boid(Flock &flock, int x, int y, bool isPredator=false);
    Boid(Flock &flock, const Vector &position, bool isPredator=false);

    /** 
     * Flying rules
     */
    void alignment(float radius, float weight);
    void cohesion(float radius, float weight);
    void separation(float radius, float weight);
    void fear(float radius, float weight);

    void update();

    /**
     * Getters
     */
    int inSight(std::function<const void(Boid &boid)> callback, float radius);
};