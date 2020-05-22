//
// Created by Maxwell James Omdal on 2/10/20.
//

#ifndef RAYCASTER_CYLINDER_H
#define RAYCASTER_CYLINDER_H


#include "Primitive.h"
#include "../../Color.h"

class Cylinder: public Primitive {
public:
    float radius;
    Point3D center;
    Vector3D orientation;
    float height;
    // Default Constructor
    Cylinder() :
        radius(1),
        center(Point3D(0,0,0)),
        orientation(Vector3D(0,0,1)),
        height(1) {
    }
    // Constructor
    Cylinder(const Point3D& center, Vector3D orientation, const float radius, const float height) :
            radius(radius),
            center(center),
            orientation(orientation.normal()),
            height(height){
    }

    float rayIntersects(Ray3D ray) override;
    Vector3D normalAtPoint(Point3D point) override;

    Color colorAtPoint(const Point3D& point) override;
};


#endif //RAYCASTER_CYLINDER_H
