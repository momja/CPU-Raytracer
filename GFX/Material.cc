//
// Created by Maxwell James Omdal on 2/15/20.
//

#include "Material.h"

Material::Material() :
    diffuseColor(Color::Red()),
    specularColor(Color::White()),
    ka(1),
    kd(1),
    ks(1),
    n(3),
    opacity(1.0),
    refraction(1.0) {
}

Material::Material(const Color &diffuseColor,
                   const Color &specularColor,
                   float ka,
                   float kd,
                   float ks,
                   float n,
                   float opacity,
                   float refraction) :
    diffuseColor(diffuseColor),
    specularColor(specularColor),
    ka(ka),
    kd(kd),
    ks(ks),
    n(n),
    opacity(opacity),
    refraction(refraction) {
}
