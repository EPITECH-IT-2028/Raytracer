#include "Plane.hpp"

Raytracer::Plane::Plane(const std::string &axis, double position,
                        const Math::Vector3D &color) {
  _color = color;

  if (axis == "X") {
    _normal = Math::Vector3D(1, 0, 0);
    _center = Math::Point3D(position, 0, 0);
  } else if (axis == "Y") {
    _normal = Math::Vector3D(0, 1, 0);
    _center = Math::Point3D(0, position, 0);
  } else if (axis == "Z") {
    _normal = Math::Vector3D(0, 0, 1);
    _center = Math::Point3D(0, 0, position);
  }
}

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
