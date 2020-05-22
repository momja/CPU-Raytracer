//
// Created by Maxwell James Omdal on 2/10/20.
//

#include "Color.h"

Color::Color(int red, int green, int blue) {
    colorArr[0] = clip(convertTo1(red), 0.f, 1.f);
    colorArr[1] = clip(convertTo1(green), 0.f, 1.f);
    colorArr[2] = clip(convertTo1(blue), 0.f, 1.f);
}

Color::Color(float red, float green, float blue) {
    colorArr[0] = clip(red,0.f,1.f);
    colorArr[1] = clip(green,0.f,1.f);
    colorArr[2] = clip(blue,0.f,1.f);
}

Color::Color(const float *channels) {
    colorArr[0] = clip(channels[0], 0, 1.f);
    colorArr[1] = clip(channels[1], 0, 1.f);
    colorArr[2] = clip(channels[2], 0, 1.f);
}

float Color::red() const {
    return colorArr[0];
}

float Color::green() const {
    return colorArr[1];
}

float Color::blue() const {
    return colorArr[2];
}

Color Color::lerp(Color color, float alpha) const {
    int newRed = int((1.0f - alpha)*float((*this).red()) + alpha*float(color.red()));
    int newGreen = int((1.0f - alpha)*float((*this).green()) + alpha*float(color.green()));
    int newBlue = int((1.0f - alpha)*float((*this).blue()) + alpha*float(color.blue()));
    return {newRed, newGreen, newBlue};
}


Color operator*(const float s, const Color &v) {
    return {v.red()*s, v.green()*s, v.blue()*s};
}

Color operator*(const Color& v, const float s) {
    return {v.red()*s, v.green()*s, v.blue()*s};
}
Color operator*(const Color& c1, const Color& c2) {
    return {c1.red()*c2.red(), c1.green()*c2.green(), c1.blue()*c2.blue()};
}


Color operator+(const Color v1, const Color &v2) {
    return {v1.red() + v2.red(), v1.green() + v2.green(), v1.blue() + v2.blue()};
}

Color operator-(const Color &v) {
    return {-v.red(), -v.green(), -v.blue()};
}

Color operator-(const Color &v1, const Color &v2) {
    return {v1.red() - v2.red(), v1.green() - v2.green(), v1.blue() - v2.blue()};
}

Color operator/(const Color &v, const float s) {
    const float invS = 1 / s;
    return {v.red()*invS, v.green()*invS, v.blue()*invS};
}
