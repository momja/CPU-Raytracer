//
// Created by Maxwell James Omdal on 2/3/20.
//

#ifndef RAYCASTER_POINT3D_H
#define RAYCASTER_POINT3D_H


class Point3D {
public:
    // Default Constructor
    Point3D();
    // Constructor given coordinates
    Point3D(float x, float y, float z);
    // Copy Constructor
    Point3D(const Point3D& point);

    virtual ~Point3D();

    // Check for "equality", taking floating point imprecision into account
    bool operator==(const Point3D& point) const;
    // Check for "inequality", taking floating point imprecision into account
    bool operator!=(const Point3D& point) const;
    // Vector assignment operator
    Point3D& operator=(const Point3D& point);

    float operator[](int i) const;
    float& operator[](int i);

    float x() const { return p[0]; }

    float y() const { return p[1]; }

    float z() const { return p[2]; }

    Point3D lerp(const Point3D &point, float alpha) const;
    Point3D jitter(float weight, int range);

private:
    // Represents the point value for homogeneous coordinates
    // neccessary for transformations
    float w = 1.0;
    // Actual point values
    float p[3]{};
};

#endif //RAYCASTER_POINT3D_H
