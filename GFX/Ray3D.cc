
//
// Created by Maxwell James Omdal on 2/10/20.
//

#include "Ray3D.h"
#include <algorithm>

Point3D Ray3D::pointAtTime(float t) {
    return origin + t*direction;
}

Ray3D Ray3D::jitterRay(float weight, int range) {
    float randX = float(rand() % range - float(range)/2.f)/weight;
    float randY = float(rand() % range - float(range)/2.f)/weight;
    float randZ = float(rand() % range - float(range)/2.f)/weight;
    Vector3D jitter = Vector3D(randX, randY, randZ);
    Vector3D jitteredDirection = ((Point3D(direction[1], direction[1], direction[2]) + jitter) - origin);
    Point3D newOrigin = origin;
    return Ray3D(jitteredDirection, newOrigin);
}
