#include "Sphere.hpp"
#include <cmath>
#include "Vector3D.hpp"

std::tuple<bool, Math::Vector3D> Raytracer::Sphere::hits(
    const Raytracer::Ray &ray) const {
  Math::Vector3D oc = ray.origin - _center;

  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * oc.dot(ray.direction);
  double c = oc.dot(oc) - _radius * _radius;
  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return {false, _color};
  }
  double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
  if (t1 < 0 && t2 < 0) {
    return {false, _color};
  }
  return {true, _color};
}
