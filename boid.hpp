#pragma once

#include "flock.hpp"
#include "vector.hpp"

#include <SFML/Graphics.hpp>
#include <deque>
#include <functional>

class Flock;

class Boid {
    sf::ConvexShape shape;
    
    Vector position;
    Vector velocity;

    int size;
    int tailLength;

    bool isPredator;
    bool isDead;

    std::deque<Vector> tail;

    Flock &flock; // Friend reference

public:
    Boid(Flock &flock, int x=0, int y=0,  bool isPredator=false);

    static sf::ConvexShape getShape();

    /** 
     * Flying rules
     */
    void alignment(float radius, float weight);
    void cohesion(float radius, float weight);
    void separation(float radius, float weight);
    void fear(float radius, float weight);

    /**
     * Drawing
     */
    void compute();
    void draw(sf::RenderWindow &window);

    /**
     * Edges
     */
    void bounce(float margin, float turnFactor);
    void wrap();

    /**
     * Getters
     */
    int inSight(std::function<const void(Boid &boid)> callback, float radius);

    float distanceTo(Boid &other, bool wrap = false);
    float angleTo(Boid &other);

    float angle();
    float speed();
};