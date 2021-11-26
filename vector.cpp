/**
 * 2D Vector in a vector space.
 */
#include "vector.hpp"

#include <cmath>

Vector::Vector() : x{0}, y{0} {}
Vector::Vector(double x, double y) : x{x}, y{y} {}

Vector Vector::operator+(const double scalar)
{
    return Vector(x * scalar, y * scalar);
}

Vector Vector::operator-(const double scalar)
{
    return Vector(x - scalar, y - scalar);
}

Vector Vector::operator*(const double scalar)
{
    return Vector(x * scalar, y * scalar);
}

Vector Vector::operator/(const double scalar)
{
    return Vector(x / scalar, y / scalar);
}

Vector Vector::operator+(const Vector &other)
{
    return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector &other)
{
    return Vector(x - other.x, y - other.y);
}

Vector Vector::operator*(const Vector &other)
{
    return Vector(x * other.x, y * other.y);
}

Vector Vector::operator/(const Vector &other)
{
    return Vector(x / other.x, y / other.y);
}

Vector &Vector::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector &Vector::operator/=(double scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector &Vector::operator+=(double scalar)
{
    x += scalar;
    y += scalar;
    return *this;
}

Vector &Vector::operator+=(Vector Vector)
{
    x += Vector.x;
    y += Vector.y;
    return *this;
}

Vector &Vector::operator-=(double scalar)
{
    x -= scalar;
    y -= scalar;
    return *this;
}

Vector &Vector::operator-=(Vector Vector)
{
    x -= Vector.x;
    y -= Vector.y;
    return *this;
}

double Vector::norm() const
{
    return sqrt(x * x + y * y);
}

Vector &Vector::normalize()
{
    float magnitude = norm();
    if (magnitude != 0)
        *this /= magnitude;
    return *this;
}

Vector &Vector::limit(double max)
{
    float magnitude = norm();
    if (magnitude > max)
        *this *= max / magnitude;
    return *this;
}

Vector &Vector::rotate(double angle)
{
    *this = Vector(
        x * cos(angle) - y * sin(angle),
        x * sin(angle) + y * cos(angle)
    );
    return *this;
}

void Vector::fmod(double mod)
{
    ::fmod(x, mod);
    ::fmod(y, mod);
}

double Vector::angle() const
{
    return ::atan2(y, x);
}

double Vector::distance(Vector other) const
{
    double dx = x - other.x;
    double dy = y - other.y;
    return sqrt(dx * dx + dy * dy);
}

/**
 * Toroidal distance in a Wrap Around space.
 * @param other Other vector point
 * @param width Width of the vector space
 * @param height Height of the vector space
 */
double Vector::toroidal_distance2(const Vector &other, double width, double height) const
{
    double dx = x - other.x;
    double dy = y - other.y;

    dx = dx > width / 2 ? width - dx : dx;
    dy = dy > height / 2 ? height - dy : dy;

    return dx * dx + dy * dy;
}

double Vector::toroidal_distance(const Vector &other, double width, double height) const
{
    return sqrt(toroidal_distance2(other, width, height));
}

double Vector::angle(const Vector &other) const
{
    return ::atan2(other.y - y, other.x - x);
}

static double frand() { return (double)rand() / RAND_MAX; }

Vector::operator std::string() const
{
    char buff[64];
    snprintf(buff, sizeof(buff), "(%.2f, %.2f)", x, y);
    return std::string(buff);
}

Vector Vector::random(double max, double offset)
{
    return Vector(frand() * max + offset, frand() * max + offset);
}
