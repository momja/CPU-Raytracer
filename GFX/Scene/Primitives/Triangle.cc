//
// Created by Maxwell James Omdal on 2/24/20.
//

#include "Triangle.h"
#include <cmath>

float Triangle::rayIntersects(Ray3D ray) {
    // Solve for plane, ray intersection
    float A = surfaceNormal[0], B = surfaceNormal[1], C = surfaceNormal[2];
    float D = -((vertices[0]*surfaceNormal).dot(Vector3D(1,1,1)));

    float x_0 = ray.origin[0], y_0 = ray.origin[1], z_0 = ray.origin[2];
    float x_d = ray.direction[0], y_d = ray.direction[1], z_d = ray.direction[2];
    float denominator = (A*x_d + B*y_d + C*z_d);
    if (abs(denominator) <= 0.0001) {
        // No ray plane intersection exists
        return -1.f;
    }
    float numerator = -(A*x_0 + B*y_0 + C*z_0 + D);

    float t = numerator/denominator;

    if (t < 0) {
        // Triangle is behind viewing plane
        return -1.f;
    }

    Point3D p = ray.pointAtTime(t);

    if (pointLiesOnTriangle(p)) {
        return t;
    } else {
        return -1;
    }
}

Vector3D Triangle::normalAtPoint(Point3D p) {
    if (!smooth) {
        return surfaceNormal;
    } else {
        // Interpolate between corners of triangle to get normal
        Vector3D ep = p - vertices[0];
        float d11 = e1.dot(e1);
        float d12 = e1.dot(e2);
        float d22 = e2.dot(e2);
        float dp1 = ep.dot(e1);
        float dp2 = ep.dot(e2);
        float D = d11*d22 - d12*d12;
        float D_beta = d22*dp1 - d12*dp2;
        float D_gamma = d11*dp2 - d12*dp1;
        float beta = D_beta/D;
        float gamma = D_gamma/D;
        float alpha = 1 - (beta + gamma);

        return (alpha*normals[0]+beta*normals[1]+gamma*normals[2]).normal();
    }
}

bool Triangle::pointLiesOnTriangle(const Point3D &p) {
    // See if point on a plane is within a triangle
    Vector3D ep = p - vertices[0];
    float d11 = e1.dot(e1);
    float d12 = e1.dot(e2);
    float d22 = e2.dot(e2);
    float dp1 = ep.dot(e1);
    float dp2 = ep.dot(e2);
    float D = d11*d22 - d12*d12;
    float D_beta = d22*dp1 - d12*dp2;
    float D_gamma = d11*dp2 - d12*dp1;
    float beta = D_beta/D;
    float gamma = D_gamma/D;
    float alpha = 1 - (beta + gamma);

    return (alpha > 0.0000001 && alpha < 1.0000001 && beta > 0.0000001 && beta < 1.0000001 && gamma > 0.0000001 && gamma < 1.0000001);
}

Color Triangle::colorAtPoint(const Point3D &p) {
    if (texture != nullptr) {
        // Interpolate between coordinate map to get color at point
        Vector3D ep = p - vertices[0];
        float d11 = e1.dot(e1);
        float d12 = e1.dot(e2);
        float d22 = e2.dot(e2);
        float dp1 = ep.dot(e1);
        float dp2 = ep.dot(e2);
        float D = d11*d22 - d12*d12;
        float D_beta = d22*dp1 - d12*dp2;
        float D_gamma = d11*dp2 - d12*dp1;
        float beta = D_beta/D;
        float gamma = D_gamma/D;
        Point3D textureCoordinate = textureCoords[0] + beta*(textureCoords[1] - textureCoords[0]) + gamma*(textureCoords[2] - textureCoords[0]);

        return texture->getPixelInTextureCoordinates(textureCoordinate[0],textureCoordinate[1]);
    }

    return material.diffuseColor;
}