#include "Plane.hpp"
#include <iostream>

/**
 * @brief Calculates the intersection of a ray with the plane.
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the intersection point (t).
 *                   Returns 0.0 if the ray is parallel to the plane or if the hit is behind the ray origin.
 *         - Math::Vector3D: The color of the plane.
 *         - const Raytracer::IShape*: A pointer to this plane object, or nullptr if no valid intersection.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Plane::hits(const Raytracer::Ray &ray) const {
  float denominator = _normal.dot(ray.direction);

  if (std::abs(denominator) > 1e-6) {
    Math::Vector3D vectorToPlane = _center - ray.origin;
    float t = vectorToPlane.dot(_normal) / denominator;
    return {t, _color, this};
  }
  return {0.0, _color, nullptr};
}

extern "C" {
/**
 * @brief Factory function to create a new Plane instance.
 *
 * This function is typically used by a plugin system to instantiate shape objects.
 * @return Raytracer::IShape* A pointer to the newly created Plane, or nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Plane();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Plane object: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
