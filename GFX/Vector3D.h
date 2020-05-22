//
// Created by Maxwell James Omdal on 2/3/20.
//

#ifndef RAYCASTER_VECTOR3D_H
#define RAYCASTER_VECTOR3D_H


#include "Point3D.h"

class Vector3D {
public:
    // Default Constructor gives vector <0,0,0>
    Vector3D();
    // Constructor given coordinates
    Vector3D(float x, float y, float z);
    // Copy Constructor
    Vector3D(const Vector3D& vector);

    virtual ~Vector3D();

    // Check for "equality", taking floating point imprecision into account
    bool operator==(const Vector3D& vector) const;
    // Check for "inequality", taking floating point imprecision into account
    bool operator!=(const Vector3D& vector) const;
    // Vector assignment operator
    Vector3D& operator=(const Vector3D& vector);

    // Gets the coordinate of vector at index (x,y,z). Can also be used to set coordinate.
    float operator[](const int i) const;
    float& operator[](const int i);

    float x() const { return v[0]; }

    float y() const { return v[1]; }

    float z() const { return v[2]; }

    float l2Norm() const;
    float l1Norm() const;

    Vector3D normal();
    Vector3D cross(const Vector3D &vector) const;
    float dot(const Vector3D &vector) const;
    Vector3D lerp(const Vector3D &vector, float alpha) const;
    Vector3D projectOnto(Vector3D v);
    static bool parallel(const Vector3D &v1, const Vector3D &v2);
private:
    // set to 0.0. Represents the 4th dimension in homogeneous coordinates
    float w = 0.0;
    // Actual vector values
    float v[3]{};

};

Vector3D operator*(const float s, const Vector3D& v);
Vector3D operator*(const Vector3D& v, const float s);
Vector3D operator*(const Vector3D& v1, const Vector3D& v2);
Vector3D operator*(const Point3D& p1, const Vector3D& v2);

Vector3D operator+(const Vector3D v1, const Vector3D& v2);
Point3D operator+(const Point3D& p, const Vector3D v);
Point3D operator+(const Vector3D v, const Point3D& p);

Vector3D operator-(const Vector3D& v);
Point3D operator-(const Point3D& p, const Vector3D& v);
Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
Vector3D operator-(const Point3D& p1, const Point3D& p2);

Vector3D operator/(const Vector3D& v, const float s);

#endif //RAYCASTER_VECTOR3D_H
