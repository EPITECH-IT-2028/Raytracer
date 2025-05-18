#include "Sphere.hpp"
#include <cmath>
#include <iostream>
#include "IShape.hpp"
#include "Vector3D.hpp"

/**
 * @brief Calculates the intersection of a ray with the sphere.
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the closest intersection point (t).
 *                   Returns 0.0 if there is no hit or if hits are behind the ray origin.
 *         - Math::Vector3D: The color of the sphere.
 *         - const Raytracer::IShape*: A pointer to this sphere object.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Sphere::hits(const Raytracer::Ray &ray) const {
  Math::Vector3D oc = ray.origin - _center;

  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * oc.dot(ray.direction);
  double c = oc.dot(oc) - _radius * _radius;
  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return {0.0, _color, this};
  }
  double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
  if (t1 < 0 && t2 < 0) {
    return {0.0, _color, this};
  }
  return {t1 < t2 ? t1 : t2, _color, this};
}

extern "C" {
/**
 * @brief Factory function to create a new Sphere instance.
 *
 * This function is typically used by a plugin system to instantiate shape objects.
 * @return Raytracer::IShape* A pointer to the newly created Sphere, or nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Sphere();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Sphere object: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
