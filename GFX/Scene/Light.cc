//
// Created by Maxwell James Omdal on 2/15/20.
//

#include "Light.h"

Light::Light(Vector3D direction, bool directional, Color color):
    directional(directional),
    color(color) {
    if (directional) {
        this->direction = direction.normal();
    } else {
        this->direction = direction;
    }
}
