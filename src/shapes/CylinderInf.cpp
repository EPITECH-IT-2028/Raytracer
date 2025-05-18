#include "CylinderInf.hpp"
#include <cmath>
#include <iostream>
#include "Vector3D.hpp"

/**
 * @brief Calculates the intersection of a ray with the infinite cylinder.
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the closest
 * intersection point (t). Returns 0.0 if there is no hit or if hits are behind
 * the ray origin.
 *         - Math::Vector3D: The color of the cylinder.
 *         - const Raytracer::IShape*: A pointer to this cylinder object.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::CylinderInf::hits(const Raytracer::Ray &ray) const {
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

  if (t1 > 0) {
    return {t1, _color, this};
  }

  return {0.0, _color, this};
}

/**
 * @brief Gets the normal vector at a given point on the infinite cylinder's
 * surface.
 * @param point The point on the cylinder's surface.
 * @return The normalized normal vector at that point, perpendicular to the
 * cylinder's axis.
 */
Math::Vector3D Raytracer::CylinderInf::getNormal(
    const Math::Point3D &point) const {
  Math::Vector3D oc = point - _center;

  double oc_dot_normal = oc.dot(_normal);

  if (oc_dot_normal <= 0.0001)
    return -_normal;

  Math::Vector3D oc_perp = (oc - (_normal * oc_dot_normal)) / _radius;
  return oc_perp.normalize();
}

/**
 * @brief Translates the cylinder by a given offset.
 * @param offset The vector by which to translate the cylinder's center.
 * rotation = v.cos(angle) + (axis X v) * sin(angle) + axis * (axis . v) * (1 -
 * cos(angle))
 */
void Raytracer::CylinderInf::rotate(const Math::Vector3D &axis, float angle) {
  double radians = angle * M_PI / 180.0;
  double cos = std::cos(radians);
  double sin = std::sin(radians);

  Math::Vector3D newNormal =
      _normal * cos + Math::cross(axis.normalized(), _normal) * sin +
      axis.normalized() * axis.normalized().dot(_normal) * (1 - cos);
  _normal = newNormal.normalize();
}

extern "C" {
/**
 * @brief Factory function to create a new CylinderInf instance.
 *
 * This function is typically used by a plugin system to instantiate shape
 * objects.
 * @return Raytracer::IShape* A pointer to the newly created CylinderInf, or
 * nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::CylinderInf();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create cylinder: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
