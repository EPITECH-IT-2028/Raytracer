#include "Plane.hpp"
#include <cmath>

bool Raytracer::Plane::hits(const Raytracer::Ray &ray) const {
  double denominator = ray.direction.dot(normal);

  if (std::abs(denominator) < std::numeric_limits<double>::epsilon()) {
    return false;
  }

  Math::Vector3D oc = center - ray.origin;
  double numerator = oc.dot(normal);

  double t = numerator / denominator;

  return t > 0;
}
