#include "Plane.hpp"

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Plane::hits(const Raytracer::Ray &ray) const {
  float denominator = _normal.dot(ray.direction);

  if (denominator == 0) {
    return {0.0, _color, this};
  }
  float t = (ray.origin - _center).dot(_normal) / denominator;
  if (t < 0) {
    return {0.0, _color, this};
  }
  return {t, _color, this};
}
