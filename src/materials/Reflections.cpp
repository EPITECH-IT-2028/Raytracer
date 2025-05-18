#include "Reflections.hpp"
#include <iostream>
#include "Vector3D.hpp"

/**
 * @brief Computes the color for a reflective material.
 *
 * If the recursion depth is not exhausted, it calculates the reflection direction
 * and casts a new ray from the hit point (slightly offset) in that direction.
 * The color obtained from this reflected ray is then blended with the object's
 * own color using a fixed reflectivity factor (0.9 for reflected, 0.1 for base color).
 *
 * @param normal The surface normal at the hit point.
 * @param viewDir The direction from the hit point to the camera.
 * @param hitPoint The point of intersection on the surface.
 * @param color The base color of the reflective material.
 * @param shapes The collection of shapes in the scene.
 * @param lights The collection of lights in the scene.
 * @param camera The camera used for rendering.
 * @param depth The current recursion depth for ray tracing.
 * @param rayColorFunc A function to compute the color of a ray.
 * @return Math::Vector3D The blended color of the reflective surface.
 */
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
/**
 * @brief Factory function to create a new Reflections material instance.
 *
 * This function is typically used by a plugin system to instantiate material objects.
 * @return Raytracer::IMaterials* A pointer to the newly created Reflections material, or nullptr on failure.
 */
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
