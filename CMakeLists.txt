cmake_minimum_required(VERSION 3.15)
project(raycaster)

set(CMAKE_CXX_STANDARD 14)

add_executable(raycaster main.cpp CLIHelper.cc CLIHelper.h GFX/Vector3D.cc GFX/Vector3D.h GFX/Point3D.cc GFX/Point3D.h GFX/PixelData.cc GFX/PixelData.h GFX/Color.cc GFX/Color.h ViewParameters.h GFX/Scene/Primitives/Sphere.cc GFX/Scene/Primitives/Sphere.h GFX/Scene/Primitives/Cylinder.cc GFX/Scene/Primitives/Cylinder.h GFX/Scene/Primitives/Primitive.cc GFX/Scene/Primitives/Primitive.h GFX/Ray3D.cc GFX/Ray3D.h GFX/Scene/SceneObject.cc GFX/Scene/SceneObject.h GFX/Scene/Light.cc GFX/Scene/Light.h GFX/Material.cc GFX/Material.h GFX/Scene/Camera.cc GFX/Scene/Camera.h GFX/Scene/Primitives/Triangle.cc GFX/Scene/Primitives/Triangle.h)