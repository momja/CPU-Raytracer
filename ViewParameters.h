//
// Created by Maxwell James Omdal on 2/10/20.
//

#include <vector>
#include "GFX/Vector3D.h"
#include "GFX/Color.h"
#include "GFX/Point3D.h"
#include "GFX/Material.h"
#include "GFX/Scene/SceneObject.h"
#include "GFX/Scene/Light.h"
#include "GFX/Scene/Primitives/Primitive.h"
#include "GFX/PixelData.h"

#ifndef RAYCASTER_VIEWPARAMETERS_H
#define RAYCASTER_VIEWPARAMETERS_H

struct ViewParameters {
    Point3D eye;
    Vector3D viewDir;
    Vector3D upDir;
    float hFOV;
    int widthPixels;
    int heightPixels;
    Color bkgColor;
    Material mtlColor;
    std::vector<PixelData> textureMaps;
    std::vector<Primitive*> objects = std::vector<Primitive*>();
    std::vector<Light*> lights = std::vector<Light*>();
    Color I_dc = Color::Black();
    float alpha_max = 1;
    float alpha_min = 1;
    float distance_max = 1000;
    float distance_min = 1000;
    float viewDist = -1;
    bool textureEnabled = false;
    std::vector<Point3D> vertexBuffer;
    std::vector<Vector3D> indexBuffer;
    std::vector<Material> polygonMaterialBuffer;
    std::vector<Vector3D> vertexNormals;
    std::vector<Point3D> textureCoordinates;
    std::vector<int> smoothShadedTriangleIndexBuffer; // Index of the face in indexBuffer of a given triangle
    std::vector<Vector3D> smoothShadedTriangleNormalIndexBuffer; // Normals indices for each vertex in a triangle. Corresponds to vertexNormals
    std::vector<int> textureIndexBuffer;
    std::vector<Vector3D> textureCoordinatesIndexBuffer;
    std::vector<int> textureIndices;
    ~ViewParameters() {
        for(auto & light : lights) {
            delete light;
        }
        for(auto & shape : objects) {
            delete shape;
        }
    }
};

#endif //RAYCASTER_VIEWPARAMETERS_H