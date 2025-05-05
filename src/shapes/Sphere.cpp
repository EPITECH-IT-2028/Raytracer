#include "Sphere.hpp"
#include <cmath>
#include "IShape.hpp"
#include "Vector3D.hpp"

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Sphere::hits(const Raytracer::Ray &ray) const {
  Math::Vector3D oc = ray.origin - _center;

  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * oc.dot(ray.direction);
  double c = oc.dot(oc) - _radius * _radius;
  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return {0.0, _color, this};
  }
  double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
  if (t1 < 0 && t2 < 0) {
    return {0.0, _color, this};
  }
  return {t1 < t2 ? t1 : t2, _color, this};
}

extern "C" {
  Raytracer::IShape *addShape() { return new Raytracer::Sphere(); }
}
