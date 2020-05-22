//
// Created by Maxwell James Omdal on 2/16/20.
//

#ifndef RAYCASTER_CAMERA_H
#define RAYCASTER_CAMERA_H


#include "../Vector3D.h"
#include "../PixelData.h"
#include "SceneObject.h"
#include "Light.h"
#include "Primitives/Primitive.h"
#include <vector>
#include <list>
#include <cstdlib>

#define SUBDIVISIONS 4
#define NEARPLANE 0.01
#define SOFT_SHADOW_RAY_COUNT 1
#define MAX_REFLECTION_RECURSIONS 1
#define DOF_SAMPLE_SIZE 20

class Camera {
public:
    Camera(const Point3D& eye, const Vector3D& upDir, const Vector3D& viewDir);
    Camera(const Point3D& eye, const Vector3D& upDir, const Vector3D& viewDir, int height, int width, float hFOV, const Color& bkgColor);
    Point3D eye;
    Vector3D upDir;
    Vector3D viewDir;
    int height;
    int width;
    float hFOV = 60.0;
    float aspectRatio;
    PixelData image;
    Color bkgColor;
    Color I_dc;
    float alpha_max = 1;
    float alpha_min = 1;
    float distance_near = 1000;
    float distance_far = 1000;
    float viewDist = -1;
    Material defaultMaterial = Material(Color::Black(), Color::Black(), 0, 0, 0, 0, 0, 1);
    void drawImage(const std::vector<Primitive*>& sceneObjects, const std::vector<Light*>& lights, bool isOdd);
private:
    Color castRay(Ray3D ray, const std::vector<Primitive*>& sceneObjects, const std::vector<Light*>& lights, std::list<Material> recursiveMaterialStack, int recursionDepth = 0);
    Color depthCue(const Color& I, const Point3D& point);
    static float fresnelCoefficient(Material m1, Material m2, const Vector3D& normal, const Vector3D& normalizedIncidentVec);
    static Vector3D reflection(const Vector3D& normal, const Vector3D& normalizedIncidentVec);
    static Vector3D transmission(const Vector3D& normal, const Vector3D& normalizedIncidentVec, Material m1, Material m2);
    float distanceFromEye(const Point3D& point);
};


#endif //RAYCASTER_CAMERA_H
