#include <tuple>
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"
#include "Triangle.hpp"
#include <iostream>

/**
 * @brief Calculates the intersection of a ray with the triangle using the Möller–Trumbore algorithm.
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the intersection point (t).
 *                   Returns 0.0 if there is no hit or if the hit is behind the ray origin.
 *         - Math::Vector3D: The color of the triangle.
 *         - const Raytracer::IShape*: A pointer to this triangle object.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *> Raytracer::Triangle::hits(
    const Raytracer::Ray &ray) const {

    Math::Vector3D edge1 = _p2 - _p1;
    Math::Vector3D edge2 = _p3 - _p1;
    Math::Vector3D h = Math::cross(ray.direction, edge2);
    double a = edge1.dot(h);

    if (a > -EPS && a < EPS)
      return {0.0, _color, this};

    double f = 1.0 / a;
    Math::Vector3D s = ray.origin - _p1;
    double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
      return {0.0, _color, this};

    Math::Vector3D q = Math::cross(s, edge1);
    double v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
      return {0.0, _color, this};

    double t = f * edge2.dot(q);

    if (t > EPS)
      return {t, _color, this};
    return {0.0, _color, this};
}

extern "C" {
/**
 * @brief Factory function to create a new Triangle instance.
 *
 * This function is typically used by a plugin system to instantiate shape objects.
 * @return Raytracer::IShape* A pointer to the newly created Triangle, or nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Triangle();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return nullptr;
  }
}
}
