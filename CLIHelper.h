//
// Created by Maxwell James Omdal on 2/3/20.
//

#include <iostream>
#include <string>
#include <map>
#include <list>
#include "ViewParameters.h"
#include "GFX/Scene/Primitives/Sphere.h"
#include "GFX/Scene/Primitives/Cylinder.h"
#include "GFX/Scene/Light.h"
#include "GFX/Material.h"

#ifndef RAYCASTER_CLIHELPER_H
#define RAYCASTER_CLIHELPER_H

class CLIHelper {
public:
    static bool is_number(const std::string& s);
    static ViewParameters getViewParams(int argc, char *argv[]);
private:
    static void missingParamsError(const std::string& failure);
    static void invalidParamsError(const std::string& failure);
    static Material readMaterialFromList(std::list<std::string>& point);
    static bool readBoolFromList(std::list<std::string>& point);
    static Point3D readPointFromList(std::list<std::string>& point);
    static Vector3D readVectorFromList(std::list<std::string>& vector);
    static Color readColorFromList(std::list<std::string>& color);
    static float readFloatFromList(std::list<std::string>& floatVal);
    static int readIntFromList(std::list<std::string>& intVal);
    static std::vector<int> readVertexNormalPair(std::list<std::string>& vertexNormalVal);
    static std::vector<int> readVertexTexturePair(std::list<std::string>& vertexTextureVal);
    static std::vector<int> readVertexTextureNormalPair(std::list<std::string>& vertexTextureNormalVal);
    static Point3D readTextureCoordinateFromList(std::list<std::string>& coord);
    static PixelData readTextureMapFromList(std::list<std::string>& texture);
};

#endif //RAYCASTER_CLIHELPER_H
