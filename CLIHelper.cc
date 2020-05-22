//
// Created by Maxwell James Omdal on 2/3/20.
//
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include "CLIHelper.h"

bool CLIHelper::is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

ViewParameters CLIHelper::getViewParams(int argc, char *argv[]) {
    struct ViewParameters params = ViewParameters();

    std::ifstream infile;
    infile.open(argv[1]);
    if (!infile) {
        // Return error if the file cannot be opened
        std::cout << "Error in opening file! Make sure you've provided a valid relative or absolute path" << std::endl;
        exit(EXIT_FAILURE);
    }
    // If file can be opened, read its content. We get the first line, check that the first word is imsize
    // and the next two are the width and height parameters
    std::vector<std::list< std::string >> parsedArgs = std::vector<std::list< std::string >>();
    std::vector< std::string > parsedArgNames;
    std::string line;
    while(std::getline(infile, line)) {
        std::string delimiter = " ";
        std::string key;
        std::string value;
        size_t pos = 0;
        pos = line.find(delimiter);
        key = line.substr(0,pos);
        line.erase(0, pos + delimiter.length());
            std::list<std::string> pargs;
            parsedArgNames.push_back(key);
            while ((pos = line.find(delimiter)) != std::string::npos) {
                value = line.substr(0, pos);
                if (!value.empty()) {
                    pargs.push_back(value);
                }
                line.erase(0, pos + delimiter.length());
            }
            pargs.push_back(line);
            parsedArgs.push_back(pargs);
    }
    infile.close();

    std::vector<std::string> expectedArgs = {"eye", "viewdir", "updir", "hfov", "imsize", "bkgcolor", "mtlcolor"};
    for (auto x: parsedArgNames) {
        if (std::find(parsedArgNames.begin(), parsedArgNames.end(), x) == parsedArgNames.end()) {
            missingParamsError("");
        }
    }

    int shapeCount = 0;

    for (int i = 0; i < parsedArgNames.size(); i++) {
        std::list<std::string> arg = parsedArgs[i];
        std::string name = parsedArgNames[i];
        // Handle Parameters
        if (name.find("#") != std::string::npos) {
            // Ignore commented lines
            continue;
        } else if (name == "eye") {
            params.eye = readPointFromList(arg);
        } else if (name == "viewdir") {
            params.viewDir = readVectorFromList(arg).normal();
        } else if (name == "updir") {
            params.upDir = readVectorFromList(arg).normal();
        } else if (name == "hfov") {
            params.hFOV = readFloatFromList(arg);
            // Check Input is safe
            if (params.hFOV >= 180) {
                std::cout << "horizontal field of view must be less than 180 degrees" << std::endl;
                exit(1);
            }
        } else if (name == "imsize") {
            params.widthPixels = readIntFromList(arg);
            params.heightPixels = readIntFromList(arg);
        } else if (name == "bkgcolor") {
            params.bkgColor = readColorFromList(arg);
        } else if (name == "mtlcolor") {
            params.mtlColor = readMaterialFromList(arg);
        } else if (name == "sphere") {
            params.objects.push_back(new Sphere(readPointFromList(arg), readFloatFromList(arg)));
            params.objects.back()->material = params.mtlColor;
            if (params.textureMaps.size() > 0 && params.textureEnabled) {
                params.objects.back()->texture = &params.textureMaps.back();
            }
        } else if (name == "cylinder") {
            params.objects.push_back(
                    new Cylinder(readPointFromList(arg), readVectorFromList(arg), readFloatFromList(arg),
                                 readFloatFromList(arg)));
            params.objects.back()->material = params.mtlColor;
            if (params.textureMaps.size() > 0) {
                params.objects.back()->texture = &params.textureMaps.back();
            }
        } else if (name == "light") {
            params.lights.push_back(new Light(readVectorFromList(arg), !readBoolFromList(arg), readColorFromList(arg)));
        } else if (name == "depthcueing") {
            params.I_dc = readColorFromList(arg);
            params.alpha_max = readFloatFromList(arg);
            params.alpha_min = readFloatFromList(arg);
            params.distance_max = readFloatFromList(arg);
            params.distance_min = readFloatFromList(arg);
        } else if (name == "v") {
            params.vertexBuffer.push_back(readPointFromList(arg));
        } else if (name == "f") {
            if (arg.front().find("//") != arg.front().npos) {
                // Use smooth shading
                params.smoothShadedTriangleIndexBuffer.push_back(params.indexBuffer.size());
                std::vector<int> vertexNormalPair = readVertexNormalPair(arg);
                Vector3D normals = Vector3D(vertexNormalPair[3], vertexNormalPair[4], vertexNormalPair[5]);
                Vector3D vertices = Vector3D(vertexNormalPair[0], vertexNormalPair[1], vertexNormalPair[2]);
                params.smoothShadedTriangleNormalIndexBuffer.push_back(normals);
                params.indexBuffer.push_back(vertices);
            } else if (std::count(arg.front().begin(), arg.front().end(), '/') == 2) {
                // Use Texture with smooth shading
                params.textureIndexBuffer.push_back(params.indexBuffer.size());
                params.smoothShadedTriangleIndexBuffer.push_back(params.indexBuffer.size());
                std::vector<int> vertexTextureNormalPair = readVertexTextureNormalPair(arg);
                Vector3D normals = Vector3D(vertexTextureNormalPair[3], vertexTextureNormalPair[4], vertexTextureNormalPair[5]);
                Vector3D vertices = Vector3D(vertexTextureNormalPair[0], vertexTextureNormalPair[1], vertexTextureNormalPair[2]);
                Vector3D textureCoords = Vector3D(vertexTextureNormalPair[6], vertexTextureNormalPair[7], vertexTextureNormalPair[8]);
                params.smoothShadedTriangleNormalIndexBuffer.push_back(normals);
                params.indexBuffer.push_back(vertices);
                params.textureCoordinatesIndexBuffer.push_back(textureCoords);
            } else if (arg.front().find("/") != arg.front().npos) {
                // Use Texture without smooth shading
                params.textureIndexBuffer.push_back(params.indexBuffer.size());
                std::vector<int> vertexTextureNormalPair = readVertexTexturePair(arg);
                Vector3D vertices = Vector3D(vertexTextureNormalPair[0], vertexTextureNormalPair[1], vertexTextureNormalPair[2]);
                Vector3D textureCoords = Vector3D(vertexTextureNormalPair[3], vertexTextureNormalPair[4], vertexTextureNormalPair[5]);
                params.indexBuffer.push_back(vertices);
                params.textureCoordinatesIndexBuffer.push_back(textureCoords);
            } else {
                // Use flat shading
                params.indexBuffer.push_back(readVectorFromList(arg));
            }
            params.textureIndices.push_back(params.textureMaps.size() - 1);
            params.polygonMaterialBuffer.push_back(params.mtlColor);
        } else if (name == "vn") {
            params.vertexNormals.push_back(readVectorFromList(arg));
        } else if (name == "vt") {
            params.textureCoordinates.push_back(readTextureCoordinateFromList(arg));
        } else if (name == "texture") {
            params.textureEnabled = true;
            params.textureMaps.push_back(readTextureMapFromList(arg));
        } else if (name == "texture-enabled") {
            params.textureEnabled = readBoolFromList(arg);
        } else if (name == "viewdist") {
            params.viewDist = readFloatFromList(arg);
        }
        else {
            continue;
        }
        // Handle case where an item has too many parameters
        if (!arg.empty()) {
            std::cout << "Too many arguments were provided for a parameter of type: " << name << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return params;
}

void CLIHelper::missingParamsError(const std::string& failure) {
    std::cout << "The provided input file is missing parameters. Execution failed while looking for parameter: " << failure << std::endl;
    exit(EXIT_FAILURE);
}

void CLIHelper::invalidParamsError(const std::string& failure) {
    std::cout << "The provided input file has invalid parameters: " << failure << std::endl;
    exit(EXIT_FAILURE);
}

Material CLIHelper::readMaterialFromList(std::list<std::string> &arg) {
    Color diffuseColor = readColorFromList(arg);
    Color specularColor = readColorFromList(arg);
    Vector3D multipliers = readVectorFromList(arg);
    float n = readFloatFromList(arg);
    float alpha = readFloatFromList(arg);
    if (alpha > 1.0 || alpha < 0.0) {
        CLIHelper::invalidParamsError("alpha not between 0 and 1");
    }
    float eta = readFloatFromList(arg);
    return {diffuseColor, specularColor, multipliers[0], multipliers[1], multipliers[2], n, alpha, eta};
}

bool CLIHelper::readBoolFromList(std::list<std::string> &arg) {
    if (arg.empty()) {
        CLIHelper::missingParamsError("bool");
    }
    std::string bStr = arg.front();
    arg.pop_front();
    bool b = (bStr == "1") || (bStr == "true") || (bStr == "True");
    return b;
}

Point3D CLIHelper::readPointFromList(std::list<std::string>& arg) {
    if (arg.size() < 3) {
        CLIHelper::missingParamsError("point");
    }
    std::string xstr = arg.front();
    arg.pop_front();
    std::string ystr = arg.front();
    arg.pop_front();
    std::string zstr = arg.front();
    arg.pop_front();
    try {
        float x = std::stof(xstr);
        float y = std::stof(ystr);
        float z = std::stof(zstr);
        return {x,y,z};
    } catch(std::invalid_argument& e) {
        std::cout << "An error occured while trying to read a point from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    } catch(std::out_of_range& e) {
        std::cout << "An error occured while trying to read a point from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    }
}

Vector3D CLIHelper::readVectorFromList(std::list<std::string>& arg) {
    if (arg.size() < 3) {
        CLIHelper::missingParamsError("vector");
    }
    std::string xstr = arg.front();
    arg.pop_front();
    std::string ystr = arg.front();
    arg.pop_front();
    std::string zstr = arg.front();
    arg.pop_front();
    try {
        float x = std::stof(xstr);
        float y = std::stof(ystr);
        float z = std::stof(zstr);
        return {x,y,z};
    } catch(std::invalid_argument& e) {
        std::cout << "An error occured while trying to read a vector from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    } catch(std::out_of_range& e) {
        std::cout << "An error occured while trying to read a vector from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    }
}

Color CLIHelper::readColorFromList(std::list<std::string>& arg) {
    if (arg.size() < 3) {
        CLIHelper::missingParamsError("color");
    }
    std::string xstr = arg.front();
    arg.pop_front();
    std::string ystr = arg.front();
    arg.pop_front();
    std::string zstr = arg.front();
    arg.pop_front();
    try {
        float x = std::stof(xstr);
        float y = std::stof(ystr);
        float z = std::stof(zstr);
        return {x,y,z};
    } catch(std::invalid_argument& e) {
        std::cout << "An error occured while trying to read a color from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    } catch(std::out_of_range& e) {
        std::cout << "An error occured while trying to read a color from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    }
}

float CLIHelper::readFloatFromList(std::list<std::string>& arg) {
    if (arg.empty()) {
        CLIHelper::missingParamsError("float");
    }
    std::string fstr = arg.front();
    arg.pop_front();
    try {
        float f = std::stof(fstr);
        return f;
    } catch(std::invalid_argument& e) {
        std::cout << "An error while trying to read a float from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    } catch(std::out_of_range& e) {
        std::cout << "An error while trying to read a float from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    }
}

int CLIHelper::readIntFromList(std::list<std::string>& arg) {
    if (arg.empty()) {
        CLIHelper::missingParamsError("int");
    }
    std::string istr = arg.front();
    arg.pop_front();
    try {
        int i = std::stoi(istr);
        return i;
    } catch(std::invalid_argument& e) {
        std::cout << "An error while trying to read an int from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    } catch(std::out_of_range& e) {
        std::cout << "An error while trying to read an int from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    }
}

std::vector<int> CLIHelper::readVertexNormalPair(std::list<std::string> &arg) {
    if (arg.size() < 3) {
        CLIHelper::missingParamsError("vertex normal pair");
    }

    std::string delimiter = "//";
    int vertex1 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    int normal1 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.length()));

    arg.pop_front();

    int vertex2 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    int normal2 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.size()));
    arg.pop_front();

    int vertex3 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    int normal3 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.size()));
    arg.pop_front();

    std::vector<int> vertexNormalPair = {vertex1, vertex2, vertex3, normal1, normal2, normal3};
    return vertexNormalPair;
}

std::vector<int> CLIHelper::readVertexTexturePair(std::list<std::string> &arg) {
    if (arg.size() < 3) {
        CLIHelper::missingParamsError("vertex normal pair");
    }

    std::string delimiter = "/";
    int vertex1 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    int texCoord1 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.length()));

    arg.pop_front();

    int vertex2 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    int texCoord2 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.size()));
    arg.pop_front();

    int vertex3 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    int texCoord3 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.size()));
    arg.pop_front();

    std::vector<int> vertexTexturePair = {vertex1, vertex2, vertex3, texCoord1, texCoord2, texCoord3};
    return vertexTexturePair;
}

std::vector<int> CLIHelper::readVertexTextureNormalPair(std::list<std::string> &arg) {
    if (arg.size() < 3) {
        CLIHelper::missingParamsError("vertex normal pair");
    }

    std::string delimiter = "/";
    int vertex1 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    arg.front().erase(0, arg.front().find(delimiter)+1);
    int texCoord1 = stoi(arg.front().substr(0, arg.front().find(delimiter)+1));
    int normal1 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.length()));
    arg.pop_front();

    int vertex2 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    arg.front().erase(0, arg.front().find(delimiter)+1);
    int texCoord2 = stoi(arg.front().substr(0, arg.front().find(delimiter)+1));
    int normal2 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.size()));
    arg.pop_front();

    int vertex3 = stoi(arg.front().substr(0, arg.front().find(delimiter)));
    arg.front().erase(0, arg.front().find(delimiter)+1);
    int texCoord3 = stoi(arg.front().substr(0, arg.front().find(delimiter)+1));
    int normal3 = stoi(arg.front().substr(arg.front().find(delimiter) + delimiter.size()));
    arg.pop_front();

    std::vector<int> vertexTextureNormalPair = {vertex1, vertex2, vertex3, normal1, normal2, normal3, texCoord1, texCoord2, texCoord3};
    return vertexTextureNormalPair;
}

Point3D CLIHelper::readTextureCoordinateFromList(std::list<std::string> &arg) {
    if (arg.size() < 2) {
        CLIHelper::missingParamsError("texture coordinate");
    }
    std::string xstr = arg.front();
    arg.pop_front();
    std::string ystr = arg.front();
    arg.pop_front();
    try {
        float x = std::stof(xstr);
        float y = std::stof(ystr);
        float z = 0;
        return {x,y,z};
    } catch(std::invalid_argument& e) {
        std::cout << "An error occurred while trying to read a vector from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    } catch(std::out_of_range& e) {
        std::cout << "An error occurred while trying to read a vector from the parameters. Make sure all values are provided correctly" << std::endl;
        exit(1);
    }
}

PixelData CLIHelper::readTextureMapFromList(std::list<std::string> &arg) {
    if (arg.empty()) {
        CLIHelper::missingParamsError("Texture map");
    }
    PixelData textureImage = PixelData::readFromFile(arg.front());
    arg.pop_front();
    return textureImage;
}
