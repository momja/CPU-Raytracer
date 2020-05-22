//
// Created by Maxwell James Omdal on 2/10/20.
//

#include <algorithm>

#ifndef RAYCASTER_COLOR_H
#define RAYCASTER_COLOR_H

class Color {
public:
    static Color Black() {
        return {0,0,0};
    }
    static Color White() {
        return {1.f,1.f,1.f};
    }
    static Color Red() {
        return {1.f,0.f,0.f};
    }
    static Color Green() {
        return {0.f,1.f,0.f};
    }
    static Color Blue() {
        return {0.f,0.f,1.f};
    }

    // Default constructor
    Color() {
        colorArr[0] = 1.f;
        colorArr[1] = 1.f;
        colorArr[2] = 1.f;
    }
    // Constructor for 0-255 colors
    Color(int red, int green, int blue);
    // Constructor for 0-1 float colors
    Color(float red, float green, float blue);
    // Constructor for array based parameters
    explicit Color(const float *channels);

    // red channel from 0-255
    float red() const;
    // green channel from 0-255
    float green() const;
    // blue channel from 0-255
    float blue() const;

    // interpolate between two colors
    Color lerp(Color color, float alpha) const;

    static int convertTo255(float val) {
        return int(val*255.f);
    }
    static float convertTo1(int val) {
        return float(val)/255.f;
    }

private:
    float colorArr [3] = {0.f,0.f,0.f};

    static float clip(float n, float lower, float upper) {
        return std::max(lower, std::min(n, upper));
    }
};

Color operator*(float s, const Color& v);
Color operator*(const Color& v, float s);
Color operator*(const Color& c1, const Color& c2);

Color operator+(Color v1, const Color& v2);

Color operator-(const Color& v);
Color operator-(const Color& v1, const Color& v2);

Color operator/(const Color& v, float s);


#endif //RAYCASTER_COLOR_H
