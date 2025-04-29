#include "Plane.hpp"

std::tuple<bool, Math::Vector3D> Raytracer::Plane::hits(
    const Raytracer::Ray &ray) const {
  float denominator = _normal.dot(ray.direction);
  if (denominator == 0) {
    return {false, _color};
  }
  float t = (ray.origin - _center).dot(_normal) / denominator;
  if (t < 0) {
    return {false, _color};
  }
  return {true, _color};
}
