/**
 * 2D Vector in a vector space.
 */
#pragma once
#include <iostream>

class Vector
{
public:
    double x;
    double y;

    Vector();
    Vector(double x, double y);

    Vector operator+(const double scalar);
    Vector operator-(const double scalar);
    Vector operator*(const double scalar);
    Vector operator/(const double scalar);

    Vector operator+(const Vector &vector);
    Vector operator-(const Vector &vector);
    Vector operator*(const Vector &vector);
    Vector operator/(const Vector &vector);

    Vector &operator*=(double scalar);
    Vector &operator/=(double scalar);
    Vector &operator+=(double scalar);
    Vector &operator+=(Vector Vector);
    Vector &operator-=(double scalar);
    Vector &operator-=(Vector Vector);

    void fmod(double mod);

    Vector &normalize();
    Vector &limit(double max = 1.0);
    Vector &rotate(double angle);
    
    double distance(Vector other) const;
    double toroidal_distance(const Vector &other, double width = 1.0, double height = 1.0) const;
    double toroidal_distance2(const Vector &other, double width = 1.0, double height = 1.0) const;
    double norm() const;
    double angle() const;
    double angle(const Vector &other) const;

    operator std::string() const;

    static Vector random(double max = 1.0, double offset = 0.0);
};
