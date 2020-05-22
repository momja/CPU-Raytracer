//
// Created by Maxwell James Omdal on 2/10/20.
//

#ifndef RAYCASTER_PRIMITIVE_H
#define RAYCASTER_PRIMITIVE_H


#include "../../Ray3D.h"
#include "../../Color.h"
#include "../SceneObject.h"
#include "../../PixelData.h"

class Primitive: public SceneObject {
public:
    PixelData *texture = nullptr;
    Material material = Material();
    Color color = Color::Red();
    virtual float rayIntersects(Ray3D ray) = 0;
    virtual Vector3D normalAtPoint(Point3D point) = 0;
    virtual Color colorAtPoint(const Point3D &point) = 0;
};

#endif //RAYCASTER_PRIMITIVE_H
