#include "Reflections.hpp"
#include "Vector3D.hpp"

Math::Vector3D Raytracer::Reflections::computeMaterial(
    const Math::Vector3D &normal, const Math::Vector3D &viewDir,
    const Math::Point3D &hitPoint, const Math::Vector3D &color,
    const Raytracer::ShapeComposite &shapes, Raytracer::LightComposite &lights,
    const Raytracer::Camera &camera, int depth,
    RayColorFunc rayColorFunc) const {
  Math::Vector3D reflectedDir = -viewDir + normal * (2 * viewDir.dot(normal));
  reflectedDir.normalize();

  Raytracer::Ray reflectedRay(hitPoint + reflectedDir * 0.001, reflectedDir);

  Math::Vector3D reflectedColor =
      rayColorFunc(reflectedRay, shapes, lights, camera, depth);

  return color * (1.0 - 0.9) + reflectedColor * 0.9;
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
