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
  double sqrt_disc = std::sqrt(discriminant);
  double t1 = (-b - sqrt_disc) / (2 * a);
  if (t1 > 0) {
    return true;
  }
  return false;
}
