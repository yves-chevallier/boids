#include "mobile.hpp"

float Mobile::speed()
{
    return velocity.norm();
}

float Mobile::angle() { return velocity.angle(); }
float Mobile::angleTo(Mobile &other) { return position.angle(other.position); }

float Mobile::distanceTo(Mobile &other)
{
    return wrap ? position.toroidal_distance(other.position)
                : position.distance(other.position);
}

/**
 * The Mobile would bounce on the edge of the map with
 * a turn factor, at a certain distance (margin) of
 * the edge.
 */
void Mobile::bounce(float margin, float turnFactor)
{
    if (position.x < margin) velocity.x += turnFactor;
    if (position.y < margin) velocity.y += turnFactor;
    if (position.x > 1.0 - margin) velocity.x -= turnFactor;
    if (position.y > 1.0 - margin) velocity.y -= turnFactor;
}

/**
 * The Mobile suddently appear at the opposite of the map
 * if it crosses the boundaries.
 */
void Mobile::wrap()
{
    if (position.x < 0) position.x += 1.0;
    if (position.y < 0) position.y += 1.0;

    if (position.x > 1.0) position.x -= 1.0;
    if (position.y > 1.0) position.y -= 1.0;
}


void Mobile::update()
{
    if (wrap)
        wrap();
    else
        bounce(speed() * 5.0, speed() / 5.0);

    velocity.limit(max_velocity);

    position += velocity;
}