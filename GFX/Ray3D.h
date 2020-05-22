//
// Created by Maxwell James Omdal on 2/10/20.
//

#ifndef RAYCASTER_RAY3D_H
#define RAYCASTER_RAY3D_H


#include "Vector3D.h"

class Ray3D {
public:
    Ray3D(Vector3D& direction, Point3D& origin):
        direction(direction.normal()),
        origin(origin) {
    }
    const Vector3D direction;
    const Point3D origin;

    Point3D pointAtTime(float t);
    Ray3D jitterRay(float weight, int range);
};


#endif //RAYCASTER_RAY3D_H
