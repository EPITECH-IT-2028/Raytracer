#include "ConeInf.hpp"
#include <cmath>
#include <iostream>
#include "Point3D.hpp"
#include "Vector3D.hpp"

/**
 * @brief Calculates the intersection of a ray with the infinite double cone.
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the closest valid intersection point (t).
 *                   Returns 0.0 if there is no hit or if hits are behind the ray origin.
 *         - Math::Vector3D: The color of the cone.
 *         - const Raytracer::IShape*: A pointer to this cone object.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::ConeInf::hits(const Raytracer::Ray &ray) const {
  Math::Vector3D cone_axis = _normal;

  Math::Vector3D oc = ray.origin - _center;
  double oc_dot_axis = oc.dot(cone_axis);
  Math::Vector3D oc_perp = oc - cone_axis * oc_dot_axis;

  double dir_dot_axis = ray.direction.dot(cone_axis);
  Math::Vector3D dir_perp = ray.direction - cone_axis * dir_dot_axis;

  double tan = std::tan(_angle);
  double tan2 = tan * tan;

  double a = dir_perp.dot(dir_perp) - tan2 * dir_dot_axis * dir_dot_axis;
  double b = 2.0 * (dir_perp.dot(oc_perp) - tan2 * dir_dot_axis * oc_dot_axis);
  double c = oc_perp.dot(oc_perp) - tan2 * oc_dot_axis * oc_dot_axis;

  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0.0) {
    return {0.0, _color, this};
  }

  double sqrt_disc = sqrt(discriminant);
  double t1 = (-b - sqrt_disc) / (2.0 * a);
  double t2 = (-b + sqrt_disc) / (2.0 * a);

  double final_t = 0.0;
  if (t1 > EPS && t2 > EPS)
    final_t = std::min(t1, t2);
  else if (t1 > EPS)
    final_t = t1;
  else if (t2 > EPS)
    final_t = t2;

  if (final_t <= EPS)
    return {0.0, _color, this};

  return {final_t, _color, this};
}


/**
 * @brief Gets the normal vector at a given point on the infinite cone's surface.
 * @param hit_point The point on the cone's surface.
 * @return The normalized normal vector at that point.
 */
Math::Vector3D Raytracer::ConeInf::getNormal(
    const Math::Point3D &hit_point) const {
  Math::Vector3D cone_axis = _normal;
  Math::Vector3D cp = hit_point - _center;

  double proj = cp.dot(cone_axis);
  Math::Point3D axis_point = _center + cone_axis * proj;
  Math::Vector3D point_to_axis = axis_point - hit_point;

  double cos_angle = cos(_angle);
  Math::Vector3D normal = point_to_axis - cone_axis * cos_angle;

  normal = normal.normalize();

  return normal;
}

void Raytracer::ConeInf::rotate(const Math::Vector3D &axis, float angle) {
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
 * @brief Factory function to create a new ConeInf instance.
 *
 * This function is typically used by a plugin system to instantiate shape objects.
 * @return Raytracer::IShape* A pointer to the newly created ConeInf, or nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::ConeInf();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create cone: " << e.what() << std::endl;
    return nullptr;
  }
}
}
