#include "Transparency.hpp"
#include <iostream>
#include "Camera.hpp"
#include "LightComposite.hpp"
#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

Math::Vector3D Raytracer::Transparency::computeMaterial(
    __attribute__((unused)) const Math::Vector3D &normal,
    const Math::Vector3D &viewDir, const Math::Point3D &hitPoint,
    const Math::Vector3D &color, const Raytracer::ShapeComposite &shapes,
    const Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
    int depth, RayColorFunc rayColorFunc) const {
  if (depth <= 0)
    return color;

  Raytracer::Ray Ray(hitPoint + -viewDir.normalized() * 0.001f,
                     -viewDir.normalized());
  Math::Vector3D Object = rayColorFunc(Ray, shapes, lights, camera, depth - 1);
  float transparency = 0.7f;
  return color * (1.0f - transparency) + Object * transparency;
}

extern "C" {
Raytracer::IMaterials *addMaterial() {
  try {
    return new Raytracer::Transparency();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Transparent material: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
