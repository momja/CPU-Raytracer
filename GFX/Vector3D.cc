//
// Created by Maxwell James Omdal on 2/3/20.
//

#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D() {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}

Vector3D::Vector3D(float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

Vector3D::Vector3D(const Vector3D &vector) {
    v[0] = vector.x();
    v[1] = vector.y();
    v[2] = vector.z();
}

Vector3D::~Vector3D() = default;

float Vector3D::operator[](const int i) const {
    if (i >= 0 && i < 3) {
        return v[i];
    }
    return w;
}

float &Vector3D::operator[](const int i) {
    return v[i];
}

float Vector3D::l2Norm() const {
    float lenSq = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    return sqrt(lenSq);
}

float Vector3D::l1Norm() const {
    return v[0] + v[1] + v[2];
}

Vector3D Vector3D::normal() {
    float len = l2Norm();
    if (len < 0.00001) {
        // if 0 vector, don't normalize
        return *this;
    }
    return Vector3D(v[0]/len, v[1]/len, v[2]/len);
}

Vector3D Vector3D::cross(const Vector3D &vector) const {
    return Vector3D(v[1] * vector[2] - v[2] * vector[1],
                   v[2] * vector[0] - v[0] * vector[2],
                   v[0] * vector[1] - v[1] * vector[0]);
}

float Vector3D::dot(const Vector3D &vector) const {
    return v[0]*vector[0] + v[1]*vector[1] + v[2]*vector[2];
}

Vector3D Vector3D::lerp(const Vector3D &vector, float alpha) const {
    return Vector3D((1.0f - alpha)*(*this)[0] + alpha*vector[0],
                    (1.0f - alpha)*(*this)[1] + alpha*vector[1],
                    (1.0f - alpha)*(*this)[2] + alpha*vector[2]);
}

bool Vector3D::parallel(const Vector3D& v1, const Vector3D& v2) {
    float theta = acos(v1.dot(v2)/(v1.l2Norm()*v2.l2Norm()));
    return abs(theta) <= 0.0001 || (theta <= 3.15 && theta >= 1.13);
}

bool Vector3D::operator==(const Vector3D &vector) const {
    return (fabs(v[0] - vector[0]) < 0.00001 &&
            fabs(v[1] - vector[1]) < 0.00001 &&
            fabs(v[2] - vector[2]) < 0.00001);
}

bool Vector3D::operator!=(const Vector3D &vector) const {
    return (fabs(v[0] - vector[0]) >= 0.00001 &&
            fabs(v[1] - vector[1]) >= 0.00001 &&
            fabs(v[2] - vector[2]) >= 0.00001);
}

Vector3D &Vector3D::operator=(const Vector3D &vector) {
    v[0] = vector[0];
    v[1] = vector[1];
    v[2] = vector[2];
    return *this;
}

Vector3D Vector3D::projectOnto(Vector3D v) {
    return (v.dot(*this))/(v.l2Norm())*(v)/v.l2Norm();
}

Vector3D operator*(const float s, const Vector3D &v) {
    return Vector3D(v[0]*s, v[1]*s, v[2]*s);
}

Vector3D operator*(const Vector3D& v, const float &s) {
    return Vector3D(v[0]*s, v[1]*s, v[2]*s);
}

Vector3D operator*(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(v1[0]*v2[0], v1[1]*v2[1], v1[2]*v2[2]);
}

Vector3D operator*(const Point3D& p1, const Vector3D& v2) {
    return Vector3D(p1[0]*v2[0], p1[1]*v2[1], p1[2]*v2[2]);
}

Vector3D operator+(const Vector3D v1, const Vector3D &v2) {
    return Vector3D(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Point3D operator+(const Point3D &p, const Vector3D v) {
    return Point3D(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
}

Point3D operator+(const Vector3D v, const Point3D &p) {
    return Point3D(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
}

Vector3D operator-(const Vector3D &v) {
    return Vector3D(-v[0], -v[1], -v[2]);
}

Point3D operator-(const Point3D &p, const Vector3D &v) {
    return Point3D(p[0] - v[0], p[1] - v[1], p[2] - v[2]);
}

Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
    return Vector3D(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vector3D operator-(const Point3D &p1, const Point3D &p2) {
    return Vector3D(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
}

Vector3D operator/(const Vector3D &v, const float s) {
    const float invS = 1 / s;
    return Vector3D(v[0]*invS, v[1]*invS, v[2]*invS);
}
