//
// Created by Maxwell James Omdal on 2/10/20.
//
#include <iostream>
#include "PixelData.h"
#include <fstream>
#include <cmath>

PixelData::PixelData(int width, int height) :
    height(height),
    width(width) {
    if (height > MAX_HEIGHT || width > MAX_WIDTH) {
        std::cout << "The max dimensions for image is 10000X10000" << std::endl;
        exit(1);
    }
    Color defaultColor = Color::Black();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setPixel(x, y, defaultColor);
        }
    }
}

PixelData::PixelData(int width, int height, Color defaultColor) :
    height(height),
    width(width) {
    if (height > MAX_HEIGHT || width > MAX_WIDTH) {
        std::cout << "The max dimensions for image is 10000X10000" << std::endl;
        exit(1);
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setPixel(x, y, defaultColor);
        }
    }
}

PixelData::~PixelData() {
//    free(rawData);
};

Color PixelData::getPixel(int x, int y) const {
    if (x < width && x >= 0 && y < height && y >= 0) {
        return Color(rawData[y*width + x]);
    }
    std::cout << "Attempting to access pixel that is not in range of image" << std::endl;
    exit(EXIT_FAILURE);
}

Color PixelData::getPixelInTextureCoordinates(float x, float y) const {
    int posY = lround(y*float(height - 1) + 0.5f);
    int posX = lround(x*float(width - 1) + 0.5f);
    return getPixel(posX, posY);
}

void PixelData::setPixel(int x, int y, Color color) {
    if (x >= width && x < 0 && y >= height && y < 0) {
        std::cout << "Attempting to change pixel that is not in range of image" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->rawData[y*width + x] = color;
}

PixelData PixelData::readFromFile(const std::string& fname) {
    std::ifstream imgFile;
    imgFile.open(fname);
    if (!imgFile) {
        // Return error if the file cannot be opened
        std::cout << "Error in opening file while trying to read ppm image! Make sure you've provided a valid relative or absolute path" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;

    std::getline(imgFile, line);

    // Make sure it is of type P3
    if (line.substr(0,line.find(' ')) != "P3") {
        std::cout << "Error while reading ppm image! Image must be of type P3" << std::endl;
    }
    line.erase(0, line.find(' ') + 1);

    int width = 0, height = 0;
    float encoding = 0;

    width = stoi(line.substr(0,line.find(' ')));
    line.erase(0, line.find(' ') + 1);

    height = stoi(line.substr(0,line.find(' ')));
    line.erase(0, line.find(' ') + 1);
    encoding = stof(line.substr(0, line.find(' ')));


    PixelData img = PixelData(width,height);
    int i = 0;
    auto* imgArr = new float[width*height*3];
    while (std::getline(imgFile, line)) {
        std::string delimiter = " ";
        std::string value;
        size_t pos = 0;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            value = line.substr(0, pos);
            if (!value.empty()) {
                imgArr[i] = stof(value);
            }
            line.erase(0, pos + delimiter.length());
            i++;
        }
        value = line.substr(0,pos);
        if (!value.empty()) {
            imgArr[i] = stof(value);
        }
        i++;
    }

    imgFile.close();

    int xPosition;
    int yPosition;

    for (int pixIndex = 0; pixIndex < width*height; pixIndex++) {
        float pix_r = imgArr[pixIndex*3];
        float pix_g = imgArr[pixIndex*3 + 1];
        float pix_b = imgArr[pixIndex*3 + 2];

        Color newColor = Color(pix_r/encoding, pix_g/encoding, pix_b/encoding);
        yPosition = (pixIndex - pixIndex%width)/width;
        xPosition = pixIndex - yPosition*width;
        img.setPixel(xPosition, yPosition, newColor);
    }


    delete[] imgArr;
    return img;
}
