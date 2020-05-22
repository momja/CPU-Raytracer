//
// Created by Maxwell James Omdal on 2/10/20.
//

#include "Sphere.h"
#include <stdio.h>
#include <cmath>
#include <iostream>

float Sphere::rayIntersects(Ray3D ray) {
    float t = -1;
    float B = 2.f*(ray.direction.x()*(ray.origin.x() - center.x()) +
                 ray.direction.y()*(ray.origin.y() - center.y()) +
                 ray.direction.z()*(ray.origin.z() - center.z()));
    float C = powf(ray.origin.x() - center.x(), 2) +
              powf(ray.origin.y() - center.y(), 2) +
              powf(ray.origin.z() - center.z(), 2)
              - powf(radius, 2);

    float discriminant = powf(B, 2) - 4*C;

    if (discriminant < 0) {
        return -1.f;
    }

    float t1 = (-B + sqrtf(discriminant))/2.f;
    float t2 = (-B - sqrtf(discriminant))/2.f;

    if (t1 > t2) std::swap(t1, t2);

    if (t1 < 0.0001) {
        t1 = t2; // if t0 is negative, let's use t1 instead
        if (t1 < 0.0001) return -1; // both t0 and t1 are negative
    }

    t = t1;

    return t;
}

Vector3D Sphere::normalAtPoint(Point3D point) {
    return (point - center)/radius;
}

Color Sphere::colorAtPoint(const Point3D &point) {
    if (texture != nullptr) {
        Vector3D centeredPoint = point - center;
        float Nx = centeredPoint.x()/radius;
        float Ny = centeredPoint.y()/radius;
        float Nz = centeredPoint.z()/radius;
        float theta, phi;
        if (Nz != 0) {
            phi = acos(Ny);
        } else {
            phi = 0;
        }
        if (Nx != 0) {
            theta = atan2(Nx, Nz);
        } else {
            theta = 0;
        }
        Point3D texCoord = Point3D((theta)/float(2*M_PI) + 0.5f, (phi)/float(M_PI), 0);
        return texture->getPixelInTextureCoordinates(texCoord.x(), texCoord.y());
    }
    return material.diffuseColor;
}