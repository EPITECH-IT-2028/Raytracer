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

  if (std::abs(denominator) > 1e-6) {
    Math::Vector3D vectorToPlane = _center - ray.origin;
    float t = vectorToPlane.dot(_normal) / denominator;
    return {t, _color, this};
  }
  return {0.0, _color, nullptr};
}
