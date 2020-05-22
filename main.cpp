#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <thread>
#include <future>
#include "GFX/Vector3D.h"
#include "GFX/PixelData.h"
#include "GFX/Ray3D.h"
#include "GFX/Point3D.h"
#include "GFX/PixelData.h"
#include "GFX/Scene/Camera.h"
#include "GFX/Scene/Primitives/Triangle.h"
#include "CLIHelper.h"

using namespace std;

void splitCamera(std::promise<PixelData> && p, const ViewParameters& params, bool isOdd) {
    Camera camera = Camera(params.eye, params.upDir, params.viewDir, params.heightPixels, params.widthPixels, params.hFOV, params.bkgColor);
    camera.I_dc = params.I_dc;
    camera.alpha_max = params.alpha_max;
    camera.alpha_min = params.alpha_min;
    camera.distance_far = params.distance_max;
    camera.distance_near = params.distance_min;
    camera.viewDist = params.viewDist;
    camera.drawImage(params.objects, params.lights, isOdd);

    p.set_value(camera.image);
}

int main(int argc, char *argv[]) {
    struct ViewParameters params = CLIHelper::getViewParams(argc ,argv);

    vector<PixelData*> textures;
    vector<Triangle*> tris;
    // Add all triangles as objects
    for (int i = 0; i < params.indexBuffer.size(); i++) {
        Vector3D vertices = params.indexBuffer[i];
        Point3D p0 = params.vertexBuffer[vertices[0] - 1];
        Point3D p1 = params.vertexBuffer[vertices[1] - 1];
        Point3D p2 = params.vertexBuffer[vertices[2] - 1];
        auto* tri = new Triangle(p0, p1, p2);
        tri->material = params.polygonMaterialBuffer[i];
        tris.push_back(tri);
        params.objects.push_back(tri);
    }

    for (int i = 0; i < params.smoothShadedTriangleIndexBuffer.size(); i++) {
        int triIndex = params.smoothShadedTriangleIndexBuffer[i];
        Vector3D n0 = params.vertexNormals[params.smoothShadedTriangleNormalIndexBuffer[i][0] - 1];
        Vector3D n1 = params.vertexNormals[params.smoothShadedTriangleNormalIndexBuffer[i][1] - 1];
        Vector3D n2 = params.vertexNormals[params.smoothShadedTriangleNormalIndexBuffer[i][2] - 1];
        tris[triIndex]->setNormals(n0, n1, n2);
        tris[triIndex]->smooth = true;
    }

    for (int i = 0; i < params.textureIndexBuffer.size(); i++) {
        int triIndex = params.textureIndexBuffer[i];
        Point3D t0 = params.textureCoordinates[params.textureCoordinatesIndexBuffer[i][0] - 1];
        Point3D t1 = params.textureCoordinates[params.textureCoordinatesIndexBuffer[i][1] - 1];
        Point3D t2 = params.textureCoordinates[params.textureCoordinatesIndexBuffer[i][2] - 1];
        textures.push_back(&params.textureMaps[params.textureIndices[triIndex]]);
        tris[triIndex]->setTexture(t0, t1, t2, textures.back());
    }

    // Handle parallel view direction and up direction
    if (Vector3D::parallel(params.viewDir, params.upDir)) {
        params.upDir = params.upDir + 0.0001 * Vector3D(1,1,1);
    }

    std::promise<PixelData> img1;
    std::promise<PixelData> img2;

    auto f1 = img1.get_future();
    auto f2 = img2.get_future();

    std::thread topImage(splitCamera, std::move(img1), std::ref(params), false);
    std::thread bottomImage(splitCamera, std::move(img2), std::ref(params), true);

    topImage.join();
    PixelData image1 = f1.get();

    bottomImage.join();
    PixelData image2 = f2.get();

    PixelData image = PixelData(image1.width, image1.height + image2.height);
    for (int y = 0; y < params.heightPixels; y+=2) {
        for (int x = 0; x < params.widthPixels; x++) {
            image.setPixel(x, y, image1.getPixel(x, y));
            if (y+1 < params.heightPixels) {
                image.setPixel(x, y + 1, image2.getPixel(x, y + 1));
            }
        }
    }

    ofstream file;
    file.open(argv[2]);

    // Write ppm header to file
    file << "P3\n" << params.widthPixels << " " << params.heightPixels << "\n" << "255" << endl;

    //  Write the pixel data to file
    for(int y = 0; y < params.heightPixels; y++) {
        for (int x = 0; x < params.widthPixels; x++) {
            Color color = image.getPixel(x, y);
            file << Color::convertTo255(color.red()) << " " << Color::convertTo255(color.green()) << " " << Color::convertTo255(color.blue()) << " ";
            if ((x + 1 + params.widthPixels*y) % 3 == 0) {
                file << endl;
            }
        }
    }
    file.close();

    return 0;
}