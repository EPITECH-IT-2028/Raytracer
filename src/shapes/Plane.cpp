#include "Plane.hpp"

bool Raytracer::Plane::hits(const Raytracer::Ray &ray) const {
  float denominator = normal.dot(ray.direction);
  if (denominator == 0) {
    return false;
  }
  float t = (ray.origin - center).dot(normal) / denominator;
  if (t < 0) {
    return false;
  }
  return true;
}
