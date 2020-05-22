//
// Created by Maxwell James Omdal on 2/15/20.
//

#ifndef RAYCASTER_MATERIAL_H
#define RAYCASTER_MATERIAL_H


#include "Color.h"

class Material {
public:
    // Default Constructor
    Material();
    // Constructor
    Material(const Color &diffuseColor,
             const Color &specularColor,
             float ka,
             float kd,
             float ks,
             float n,
             float opacity,
             float refraction);

    Color diffuseColor;
    Color specularColor;
    float ka;
    float kd;
    float ks;
    float n;
    float opacity;
    float refraction;
};


#endif //RAYCASTER_MATERIAL_H
