//
// Created by Maxwell James Omdal on 2/10/20.
//

#include "Cylinder.h"
#include <cmath>

float Cylinder::rayIntersects(Ray3D ray) {
    float t = -1;
    Vector3D v = ray.direction;
    Point3D p = ray.origin;
    Point3D pa = center;
    Vector3D va = orientation;
    float A = pow((v - (v.dot(va)*va)).l2Norm(),2);
    float B = 2*((v-v.dot(va)*va).dot(p-pa-((p-pa).dot(va)*va)));
    float C = pow((p-pa-((p-pa).dot(va)*va)).l2Norm(),2) - pow(radius, 2);

    float discriminant = powf(B, 2) - 4*A*C;

    if (discriminant < 0) {
        return -1.f;
    }

    float t1 = (-B + sqrtf(discriminant))/(2*A);
    float t2 = (-B - sqrtf(discriminant))/(2*A);

    // Take the first intersection
    if (t1 < 0 && t2 < 0) {
        return -1.f;
    }

    if (t1 < t2 && t1 > 0) {
        t = t1;
    } else if (t2 < t1 && t2 > 0) {
        t = t2;
    }

    // Cap the height of the cylinder
    Point3D q = p+t*v;
    Point3D p1 = center - height/2*orientation;
    Point3D p2 = center + height/2*orientation;
    if (va.dot(q - p1) <= 0 || va.dot(q - p2) >= 0) {
        return -1.f;
    }

    return t;
}

Vector3D Cylinder::normalAtPoint(Point3D point) {
    return ((point - center) - (orientation.dot(point - center)*orientation)).normal();
}

Color Cylinder::colorAtPoint(const Point3D& point) {
    return material.diffuseColor;
}

