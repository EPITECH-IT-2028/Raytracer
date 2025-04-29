#include "Sphere.hpp"
#include <cmath>

bool Raytracer::Sphere::hits(const Raytracer::Ray &ray) const {
  Math::Vector3D oc = ray.origin - center;

  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * oc.dot(ray.direction);
  double c = oc.dot(oc) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return false;
  }
  double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
  if (t1 < 0 && t2 < 0) {
    return false;
  }
  return true;
}
