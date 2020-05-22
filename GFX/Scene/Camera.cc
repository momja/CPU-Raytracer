//
// Created by Maxwell James Omdal on 2/16/20.
//

#include "Camera.h"
#include "../Ray3D.h"
#include <vector>
#include <algorithm>
#include "Primitives/Sphere.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <thread>

void Camera::drawImage(const std::vector<Primitive*>& sceneObjects, const std::vector<Light*>& lights, bool isOdd) {
    float widthNearPlane = 2.f*NEARPLANE*tan((hFOV*M_PI/180.0)/2.0);
    float heightNearPlane = widthNearPlane/aspectRatio;

    Point3D focalPoint = eye + viewDist*viewDir;;

    // If a value for viewDir is provided, then we need to cast multiple rays for each Subdivision
    // with jittered eye positions
    int samplingSize = DOF_SAMPLE_SIZE;
    if (viewDist < 0.0001) {
        samplingSize = 1;
        focalPoint = eye + viewDir;
    }

    for (int n = 0; n < samplingSize; n++) {
        Point3D jitteredEye;
        if (samplingSize != 1) {
            jitteredEye = eye.jitter(5000, 200);
        } else {
            jitteredEye = eye;
        }

        viewDir = (focalPoint - jitteredEye).normal();

        Vector3D u = viewDir.cross(upDir).normal();
        Vector3D v = u.cross(viewDir).normal();
        // Define the corners of the view window
        Point3D upperLeft = jitteredEye + NEARPLANE*viewDir - (widthNearPlane/2)*u + (heightNearPlane/2)*v;
        Point3D upperRight = jitteredEye + NEARPLANE*viewDir + (widthNearPlane/2)*u + (heightNearPlane/2)*v;
        Point3D lowerLeft = jitteredEye + NEARPLANE*viewDir - (widthNearPlane/2)*u - (heightNearPlane/2)*v;
        Point3D lowerRight = jitteredEye + NEARPLANE*viewDir + (widthNearPlane/2)*u - (heightNearPlane/2)*v;
        // Define the steps taken to properly map to a pixel
        Vector3D hStep = (lowerLeft - upperLeft)/(float(height - 1));
        Vector3D wStep = (upperRight - upperLeft)/(float(width - 1));
        Vector3D hiStep = (lowerLeft - upperLeft)/(float(height - 1)*SUBDIVISIONS);
        Vector3D wiStep = (upperRight - upperLeft)/(float(width - 1)*SUBDIVISIONS);

        Vector3D hOffset = Vector3D();
        Vector3D wOffset = Vector3D();
        Vector3D hiOffset = Vector3D();
        Vector3D wiOffset = Vector3D();

        //  Loop through each pixel in the image, find the color, and assign in imageData
        for (int y = isOdd; y < height; y += 2) {
            hOffset = float(y) * hStep;
            for (int x = 0; x < width; x++) {
                wOffset = float(x) * wStep;
                Color finalColor = Color::Black();
                // Use blue noise technique to shoot multiple rays per pixel
                for (int yi = 0; yi < SUBDIVISIONS; yi++) {
                    hiOffset = hiStep / 2.f + float(yi) * hiStep;
                    for (int xi = 0; xi < SUBDIVISIONS; xi++) {
                        wiOffset = wiStep / 2.f + float(xi) * wiStep;
                        // 1) Define the current point in world space
                        Point3D currentPoint = upperLeft + hOffset + wOffset + hiOffset + wiOffset;
                        // 2) Define a ray from the eye to the currentPoint
                        Vector3D direction = currentPoint - jitteredEye;
                        Ray3D ray = Ray3D(direction, currentPoint);
#if SUBDIVISIONS > 1
                        Ray3D jitteredRay = ray.jitterRay(1000, 200);
#else
                        Ray3D jitteredRay = ray;
#endif
                        std::list<Material> materialStack;
                        materialStack.push_back(defaultMaterial);
                        Color tmpColor = castRay(ray, sceneObjects, lights, materialStack);
                        finalColor =
                                finalColor + float(1.f / float(SUBDIVISIONS * SUBDIVISIONS)) * tmpColor;
                    }
                }
                Color newColor = image.getPixel(x,y) + finalColor/samplingSize;
                image.setPixel(x, y, newColor);
            }
            std::cout << "Row " << y + 1 << " completed" << std::endl;
        }
    }
}

Color Camera::castRay(Ray3D ray, const std::vector<Primitive*>& sceneObjects, const std::vector<Light*>& lights, std::list<Material> recursiveMaterialStack, int recursionDepth) {
    // Find the closest object and get its material and intersection point
    Primitive* closestObject = nullptr;
    float minT = MAXFLOAT;
    for (auto object : sceneObjects) {
        float t = object->rayIntersects(ray);
        if (t > 0.0001 && t < minT) {
            minT = t;
            closestObject = object;
        };
    }
    if (closestObject == nullptr) {
        // No objects hit by ray. No need to perform any more computation
        return bkgColor;
    }
    Point3D intersectionPoint = ray.pointAtTime(minT);
    Vector3D surfaceNormal = closestObject->normalAtPoint(intersectionPoint);

    // When exiting the back of a transparent object, negate the normal, and pop that material from the material stack
    Material currentMaterial = recursiveMaterialStack.back();
    Material transmittedMaterial = closestObject->material;
    if (surfaceNormal.dot(-ray.direction) <= 0.00001 && recursionDepth >= 1) {
        surfaceNormal = -surfaceNormal;
        currentMaterial = closestObject->material;
        if (recursiveMaterialStack.size() > 1) {
            recursiveMaterialStack.pop_back();
        }
        transmittedMaterial = recursiveMaterialStack.back();
    }

    // ----------- Compute Direct Light ---------- //

    Color ambient = Color::Black(), diffuse = Color::Black(), specular = Color::Black();

    // Compute ambient light
    ambient = ambient + transmittedMaterial.ka * closestObject->colorAtPoint(intersectionPoint);

    // Compute Diffuse and Specular Light
    for (auto light : lights) {
        Color totalDiffuseForLight = Color::Black();
        Color totalSpecularForLight = Color::Black();
        Vector3D lightDirection;
        if (light->directional) {
            float shadowFlag = 1;
            lightDirection = -1.f*light->direction;
            Ray3D lightRay = Ray3D(lightDirection, intersectionPoint);
            // Check if there is an object blocking this light
            for (auto object : sceneObjects) {
                float t = object->rayIntersects(lightRay);
                if (t > 0.0001) {
                    shadowFlag = shadowFlag * (1 - object->material.opacity);
                }
            }
            if (shadowFlag > 0.0001) {
                Vector3D halfwayVec = (-viewDir + lightDirection).normal();
                totalDiffuseForLight = totalDiffuseForLight + (fmax(0.f, (surfaceNormal.dot(lightDirection))) * transmittedMaterial.kd * (closestObject->colorAtPoint(intersectionPoint) * light->color));
                totalDiffuseForLight = totalDiffuseForLight * shadowFlag;
                totalSpecularForLight = totalSpecularForLight + ((pow(fmax(0.f, (intersectionPoint - eye).dot(halfwayVec)), transmittedMaterial.n)) * transmittedMaterial.ks * transmittedMaterial.specularColor * light->color);
                totalSpecularForLight = totalSpecularForLight * shadowFlag;
            }
        } else {
            float shadowFlag = 0;
            int i = 0;
            lightDirection = ((Point3D(light->direction[0], light->direction[1], light->direction[2])) - intersectionPoint).normal();
            while(i < SOFT_SHADOW_RAY_COUNT) {
                // Jitter light for soft shadows
                Ray3D lightRay = Ray3D(lightDirection, intersectionPoint);
#if SOFT_SHADOW_RAY_COUNT > 1
                Ray3D jitteredLightRay = lightRay.jitterRay(500.f, SOFT_SHADOW_RAY_COUNT);
#else
                Ray3D jitteredLightRay = lightRay;
# endif
                float shadowFlagForRay = 1;
                // Check if there is an object blocking this light
                for (auto object : sceneObjects) {
                    float t = object->rayIntersects(jitteredLightRay);
                    if (t > 0.0001) {
                        shadowFlagForRay = shadowFlagForRay * (1 - object->material.opacity);
                    }
                }
                shadowFlag += shadowFlagForRay/SOFT_SHADOW_RAY_COUNT;
                i++;
            }
            Vector3D halfwayVec = (-viewDir + lightDirection).normal();
            totalDiffuseForLight = totalDiffuseForLight +
                    shadowFlag * (fmax(0.f, (surfaceNormal.dot(lightDirection))) * transmittedMaterial.kd * (closestObject->colorAtPoint(intersectionPoint) * light->color));
            totalSpecularForLight = totalSpecularForLight +
                    shadowFlag*((pow(fmax(0.f, surfaceNormal.dot(halfwayVec)), transmittedMaterial.n)) * transmittedMaterial.ks * (transmittedMaterial.specularColor * light->color));
        }
        diffuse = diffuse + totalDiffuseForLight;
        specular = specular + totalSpecularForLight;
    }

    Color IDirect = ambient + diffuse + specular;


    // ----------- Compute Reflected and Transmitted Light ---------- //

    Color reflectedColor = Color::Black();
    Color transmittedColor = Color::Black();

    if (recursionDepth < MAX_REFLECTION_RECURSIONS) {
        recursionDepth++;

        float fresnelCoeff = fresnelCoefficient(currentMaterial, transmittedMaterial, surfaceNormal, ray.direction);

        // Transmitted Light
        if (transmittedMaterial.opacity < 0.9999) {
            float opacity = transmittedMaterial.opacity;
            Vector3D T = transmission(surfaceNormal, ray.direction, currentMaterial, transmittedMaterial);
            if (T.l2Norm() > 0.0001) {
                recursiveMaterialStack.push_back(transmittedMaterial);
                // Don't iterate when total internal reflection occurs
                Ray3D transmittedRay = Ray3D(T, intersectionPoint);
                transmittedColor = castRay(transmittedRay, sceneObjects, lights, recursiveMaterialStack,
                                           recursionDepth);
                transmittedColor = (1 - fresnelCoeff) * (exp(-opacity*minT)) * transmittedColor;
                recursiveMaterialStack.pop_back();
            }
        }

        // Reflected Light
        if (transmittedMaterial.refraction > 1) {
            Vector3D reflectedVector = reflection(surfaceNormal, ray.direction);
            Ray3D reflectionRay = Ray3D(reflectedVector, intersectionPoint);
            reflectedColor = castRay(reflectionRay, sceneObjects, lights, recursiveMaterialStack, recursionDepth);
            reflectedColor = fresnelCoeff * reflectedColor;
        }
    } else {
        reflectedColor = Color::Black();
        transmittedColor = Color::Black();
    }


    Color IReflected = reflectedColor;
    Color ITransmitted = transmittedColor;


    // Sum all color components
    Color I = IDirect + IReflected + ITransmitted;

    // Run depth cue to dim further away objects
    I = depthCue(I, intersectionPoint);

    return I;
}

Camera::Camera(const Point3D& eye, const Vector3D& upDir, const Vector3D& viewDir):
    eye(eye),
    upDir(upDir),
    viewDir(viewDir),
    image(PixelData(100,100)),
    height(100),
    width(100),
    aspectRatio(1),
    bkgColor(Color::White()) {
}

Camera::Camera(const Point3D& eye, const Vector3D& upDir, const Vector3D& viewDir, int height, int width, float hFOV, const Color& bkgColor):
    eye(eye),
    upDir(upDir),
    viewDir(viewDir),
    image(PixelData(width,height)),
    height(height),
    width(width),
    hFOV(hFOV),
    aspectRatio(float(width)/float(height)),
    bkgColor(bkgColor) {
}

float Camera::distanceFromEye(const Point3D &point) {
    return (point - eye).l2Norm();
}

Color Camera::depthCue(const Color& I, const Point3D& point) {
    float distance = distanceFromEye(point);
    Color I_prime;

    float alpha_dc;
    if (distance <= distance_near) {
        alpha_dc = alpha_max;
    } else if (distance_near <= distance && distance <= distance_far) {
        alpha_dc = alpha_min + (alpha_max - alpha_min)*((distance_far - distance)/(distance_far - distance_near));
    } else {
        alpha_dc = alpha_min;
    }

    I_prime = alpha_dc*I + (1 - alpha_dc)*I_dc;
    return I_prime;
}

float Camera::fresnelCoefficient(Material m1, Material m2, const Vector3D& normal, const Vector3D& normalizedIncidentVec) {
    // Calculate Fresnel Coefficient using the Schlick approximation
    Vector3D reversedNormalizedIncidentVec = -normalizedIncidentVec;
    float cosTheta = normal.dot(reversedNormalizedIncidentVec);
    float f0 = powf((m1.refraction - m2.refraction)/(m1.refraction + m2.refraction),2);
    float fr = f0 + (1.f - f0)*powf(1 - cosTheta, 5);
    return fr;
}

Vector3D Camera::reflection(const Vector3D& normal, const Vector3D& normalizedIncidentVec) {
    Vector3D reversedNormalizedIncidentVec = -1.0 * normalizedIncidentVec;
    Vector3D r = 2*(normal.dot(reversedNormalizedIncidentVec))*normal - reversedNormalizedIncidentVec;
    return r;
}

Vector3D Camera::transmission(const Vector3D &normal, const Vector3D &normalizedIncidentVec, Material m1, Material m2) {
    Vector3D reversedNormalizedIncidentVec = -1.0 * normalizedIncidentVec;
    float sinTheta = 1 - pow(normal.dot(reversedNormalizedIncidentVec), 2);
    if (sinTheta > m2.refraction/m1.refraction) {
        // This occurs when total internal reflection occurs
        // No transmission, only reflection
        return {0,0,0};
    }
//    float cosTheta = sqrt(1 - pow(m1.refraction/m2.refraction, 2)*sinTheta);
    Vector3D T = sqrt(1 - pow(m1.refraction/m2.refraction, 2)*(1 - pow(normal.dot(reversedNormalizedIncidentVec), 2)))*(-1 * normal) +
            (m1.refraction/m2.refraction)*((normal.dot(reversedNormalizedIncidentVec)*normal - reversedNormalizedIncidentVec));
    return T;
}
