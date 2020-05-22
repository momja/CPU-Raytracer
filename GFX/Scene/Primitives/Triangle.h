//
// Created by Maxwell James Omdal on 2/24/20.
//

#ifndef RAYCASTER_TRIANGLE_H
#define RAYCASTER_TRIANGLE_H


#include "Primitive.h"

class Triangle: public Primitive {
public:
    Vector3D surfaceNormal;
    float triangleArea;
    Vector3D e1;
    Vector3D e2;
    bool smooth = false;
    // Constructor
    Triangle(const Point3D& p0, const Point3D& p1, const Point3D& p2) {
        vertices[0] = p0;
        vertices[1] = p1;
        vertices[2] = p2;
        surfaceNormal = (p1 - p0).cross(p2 - p0).normal();
        e1 = p1 - p0;
        e2 = p2 - p0;
        triangleArea = 0.5f*(e1.cross(e2)).l2Norm();
        texture = nullptr;
    }
    Triangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, Vector3D n0, Vector3D n1, Vector3D n2) {
        vertices[0] = p0;
        vertices[1] = p1;
        vertices[2] = p2;
        normals[0] = n0.normal();
        normals[1] = n1.normal();
        normals[2] = n2.normal();

        surfaceNormal = (p1 - p0).cross(p2 - p0).normal();

        e1 = p1 - p0;
        e2 = p2 - p0;
        triangleArea = 0.5f*(e1.cross(e2)).l2Norm();
        texture = nullptr;
    }
    Triangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, Vector3D n0, Vector3D n1, Vector3D n2, PixelData* tex) {
        vertices[0] = p0;
        vertices[1] = p1;
        vertices[2] = p2;
        normals[0] = n0.normal();
        normals[1] = n1.normal();
        normals[2] = n2.normal();

        surfaceNormal = (p1 - p0).cross(p2 - p0).normal();

        e1 = p1 - p0;
        e2 = p2 - p0;
        triangleArea = 0.5f*(e1.cross(e2)).l2Norm();
        texture = tex;
    }
    void setNormals(Vector3D n0, Vector3D n1, Vector3D n2) {
        normals[0] = n0.normal();
        normals[1] = n1.normal();
        normals[2] = n2.normal();
    }
    void setTexture(const Point3D& t0, const Point3D& t1, const Point3D& t2, PixelData* tex) {
        textureCoords[0] = t0;
        textureCoords[1] = t1;
        textureCoords[2] = t2;
        texture = tex;
    }
    float rayIntersects(Ray3D ray) override;
    Vector3D normalAtPoint(Point3D point) override;
private:
    Point3D textureCoords [3]{};
    Point3D vertices [3]{};
    Vector3D normals [3]{};
    bool  pointLiesOnTriangle(const Point3D& p);
    Color colorAtPoint(const Point3D &p) override;
};


#endif //RAYCASTER_TRIANGLE_H
