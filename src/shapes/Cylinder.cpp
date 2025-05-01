#include "Cylinder.hpp"
#include <cmath>
#include "Vector3D.hpp"

Math::Vector3D Raytracer::Cylinder::getNormal(
    const Math::Point3D &point) const {
  Math::Vector3D oc = point - _center;
  double oc_dot_normal = oc.dot(_normal);
  Math::Vector3D oc_perp = oc - (_normal * oc_dot_normal);
  return oc_perp.normalize();
}

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
