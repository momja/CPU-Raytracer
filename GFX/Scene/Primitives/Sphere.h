//
// Created by Maxwell James Omdal on 2/10/20.
//

#include "../../Point3D.h"
#include "Primitive.h"
#include "../../Color.h"

#ifndef RAYCASTER_SPHERE_H
#define RAYCASTER_SPHERE_H


class Sphere: public Primitive {
public:
    float radius;
    Point3D center;
    // Default Constructor
    Sphere() :
        radius(1),
        center(Point3D(0,0,0)) {
    }
    // Constructor
    Sphere(const Point3D& center, const float radius) :
        radius(radius),
        center(center) {
    }

    float rayIntersects(Ray3D ray) override;
    Vector3D normalAtPoint(Point3D point) override;

    Color colorAtPoint(const Point3D& point) override;
};


#endif //RAYCASTER_SPHERE_H
