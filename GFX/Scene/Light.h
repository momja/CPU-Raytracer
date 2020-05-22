//
// Created by Maxwell James Omdal on 2/15/20.
//

#ifndef RAYCASTER_LIGHT_H
#define RAYCASTER_LIGHT_H


#include "SceneObject.h"
#include "../Vector3D.h"
#include "../Color.h"

class Light: public SceneObject {
public:
    Light(Vector3D direction, bool directional, Color color);
    Vector3D direction;
    bool directional;
    Color color;
};


#endif //RAYCASTER_LIGHT_H
