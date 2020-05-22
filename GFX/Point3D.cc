//
// Created by Maxwell James Omdal on 2/3/20.
//

#include "Point3D.h"
#include <cmath>
#include <algorithm>

Point3D::Point3D() {
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;
}

Point3D::Point3D(float x, float y, float z) {
    p[0] = x;
    p[1] = y;
    p[2] = z;
}

Point3D::Point3D(const Point3D &point) {
    p[0] = point[0];
    p[1] = point[1];
    p[2] = point[2];
}

Point3D Point3D::lerp(const Point3D &point, float alpha) const {
    float x = (1.0f - alpha)*(*this)[0] + alpha*point[0];
    float y = (1.0f - alpha)*(*this)[1] + alpha*point[1];
    float z = (1.0f - alpha)*(*this)[2] + alpha*point[2];

    return Point3D(x,y,z);
}

Point3D Point3D::jitter(float weight, int range) {
    float randX = float(rand() % range - float(range)/2.f)/weight;
    float randY = float(rand() % range - float(range)/2.f)/weight;
    float randZ = float(rand() % range - float(range)/2.f)/weight;
    Point3D jitter = Point3D(p[0] + randX, p[1] + randY, p[2] + randZ);
    return jitter;
}

Point3D::~Point3D() = default;

float Point3D::operator[](const int i) const {
    if (i >= 0 && i < 3) {
        return p[i];
    }
    return w;
}

float &Point3D::operator[](const int i) {
    return p[i];
}

bool Point3D::operator==(const Point3D &point) const {
    return (fabs(p[0] - point[0]) < 0.00001 &&
            fabs(p[1] - point[1]) < 0.00001 &&
            fabs(p[2] - point[2]) < 0.00001);
}

bool Point3D::operator!=(const Point3D &point) const {
    return (fabs(p[0] - point[0]) >= 0.00001 &&
            fabs(p[1] - point[1]) >= 0.00001 &&
            fabs(p[2] - point[2]) >= 0.00001);
}

Point3D &Point3D::operator=(const Point3D &point) {
    p[0] = point[0];
    p[1] = point[1];
    p[2] = point[2];
    return *this;
}
