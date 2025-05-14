#include "CylinderInf.hpp"
#include <cmath>
#include <iostream>
#include "Vector3D.hpp"

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::CylinderInf::hits(const Raytracer::Ray &ray) const {
  Math::Vector3D oc = ray.origin - _center;

  double oc_dot_normal = oc.dot(_normal);
  double dir_dot_normal = ray.direction.dot(_normal);

  Math::Vector3D dir_perp = ray.direction - (_normal * dir_dot_normal);
  Math::Vector3D oc_perp = oc - (_normal * oc_dot_normal);

  double a = dir_perp.dot(dir_perp);
  double b = 2.0 * dir_perp.dot(oc_perp);
  double c = oc_perp.dot(oc_perp) - _radius * _radius;

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0.0) {
    return {0.0, _color, this};
  }

  double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
  double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

  if (t1 > t2) {
    std::swap(t1, t2);
  }

  if (t1 > 0) {
    return {t1, _color, this};
  }

  return {0.0, _color, this};
}

Math::Vector3D Raytracer::CylinderInf::getNormal(
    const Math::Point3D &point) const {
  Math::Vector3D oc = point - _center;

  double oc_dot_normal = oc.dot(_normal);

  if (oc_dot_normal <= 0.0001)
    return -_normal;

  Math::Vector3D oc_perp = (oc - (_normal * oc_dot_normal)) / _radius;
  return oc_perp.normalize();
}

extern "C" {
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::CylinderInf();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create cylinder: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
