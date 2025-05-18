#include "Cylinder.hpp"
#include <cmath>
#include <iostream>
#include "Vector3D.hpp"

/**
 * @brief Calculates the intersection of a ray with the finite cylinder (body and caps).
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the closest valid intersection point (t).
 *                   Returns 0.0 if there is no hit or if hits are behind the ray origin.
 *         - Math::Vector3D: The color of the cylinder.
 *         - const Raytracer::IShape*: A pointer to this cylinder object.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Cylinder::hits(const Raytracer::Ray &ray) const {
  Math::Vector3D oc = ray.origin - _center;

  double oc_dot_normal = oc.dot(_normal);
  double dir_dot_normal = ray.direction.dot(_normal);

  Math::Vector3D dir_perp = ray.direction - (_normal * dir_dot_normal);
  Math::Vector3D oc_perp = oc - (_normal * oc_dot_normal);

  double a = dir_perp.dot(dir_perp);
  double b = 2.0 * dir_perp.dot(oc_perp);
  double c = oc_perp.dot(oc_perp) - _radius * _radius;

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0.0) {
    return {0.0, _color, this};
  }

  double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
  double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

  if (t1 > t2) {
    std::swap(t1, t2);
  }

  double t_plane_A = -oc_dot_normal / dir_dot_normal;
  double t_plane_B = (_height - oc_dot_normal) / dir_dot_normal;

  double t3 = std::min(t_plane_A, t_plane_B);
  double t4 = std::max(t_plane_A, t_plane_B);

  double final_t_start = std::max(t1, t3);
  double final_t_end = std::min(t2, t4);

  if (final_t_start < final_t_end && final_t_end >= 0) {
    return {final_t_start, _color, this};
  }

  return {0.0, _color, this};
}

/**
 * @brief Gets the normal vector at a given point on the cylinder's surface.
 * This includes the cylindrical body and the flat end caps.
 * @param point The point on the cylinder's surface.
 * @return The normalized normal vector at that point.
 */
Math::Vector3D Raytracer::Cylinder::getNormal(
    const Math::Point3D &point) const {
  Math::Vector3D oc = point - _center;

  double oc_dot_normal = oc.dot(_normal);

  if (oc_dot_normal <= 0.0001) {
    return -_normal;
  } else if (oc_dot_normal >= (_height - 0.0001)) {
    return _normal;
  }

  Math::Vector3D oc_perp = (oc - (_normal * oc_dot_normal)) / _radius;
  return oc_perp.normalize();
}

extern "C" {
/**
 * @brief Factory function to create a new Cylinder instance.
 *
 * This function is typically used by a plugin system to instantiate shape objects.
 * @return Raytracer::IShape* A pointer to the newly created Cylinder, or nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Cylinder();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create cylinder: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
