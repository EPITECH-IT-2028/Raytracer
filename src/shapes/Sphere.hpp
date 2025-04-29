#pragma once

#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

class Sphere {
public:
  Math::Point3D center;
  double radius;
  Math::Vector3D color;

  Sphere(const Math::Point3D &center, double radius)
      : center(center), radius(radius), color({0,0,0}) {}
  Sphere(const Math::Point3D &center, double radius, Math::Vector3D color)
      : center(center), radius(radius), color(color) {}
  Sphere() : center(Math::Point3D()), radius(0.0) {}

  ~Sphere() = default;

  double hits(const Raytracer::Ray &ray) const;
};

} // namespace Raytracer
