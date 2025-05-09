#include "Reflections.hpp"
#include <iostream>
#include "Vector3D.hpp"

Math::Vector3D Raytracer::Reflections::computeMaterial(
    const Math::Vector3D &normal, const Math::Vector3D &viewDir,
    const Math::Point3D &hitPoint, const Math::Vector3D &color,
    const Raytracer::ShapeComposite &shapes,
    const Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
    int depth, RayColorFunc rayColorFunc) const {
  if (depth <= 0) {
    return color;
  }

  Math::Vector3D reflectedDir = -viewDir + normal * (2 * viewDir.dot(normal));
  reflectedDir.normalize();

  Raytracer::Ray reflectedRay(hitPoint + reflectedDir * 0.001, reflectedDir);

  Math::Vector3D reflectedColor =
      rayColorFunc(reflectedRay, shapes, lights, camera, depth - 1);

  return color * (1.0f - 0.9f) + reflectedColor * 0.9f;
}

extern "C" {
Raytracer::IMaterials *addMaterial() {
  try {
    return new Raytracer::Reflections();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Reflections material: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
