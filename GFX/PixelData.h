//
// Created by Maxwell James Omdal on 2/10/20.
//

#include "Color.h"

#ifndef RAYCASTER_PIXELDATA_H
#define RAYCASTER_PIXELDATA_H

#define MAX_WIDTH 10000
#define MAX_HEIGHT 10000
#define CHANNELS 3

class PixelData {
public:
    int width;
    int height;
    // Default Constructor
    PixelData(int width, int height);
    // Color Constructor
    PixelData(int width, int height, Color defaultColor);
    // Destructor
    ~PixelData();
    // return an array of size 3 that represents the rgb color values at a pixel
    Color getPixel(int x, int y) const;
    Color getPixelInTextureCoordinates(float x, float y) const;
    // set the pixel value at x y coordinate
    void setPixel(int x, int y, Color color);
    static PixelData readFromFile(const std::string& fname);
private:
    Color *rawData = (Color *)malloc(width*height*sizeof(Color));
};


#endif //RAYCASTER_PIXELDATA_H
