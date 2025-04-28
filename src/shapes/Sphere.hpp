#pragma once

#include "Point3D.hpp"
#include "Ray.hpp"

namespace Raytracer {

class Sphere {
public:
  Math::Point3D center;
  double radius;

  Sphere(const Math::Point3D &center, double radius)
      : center(center), radius(radius) {}
  Sphere() : center(Math::Point3D()), radius(0.0) {}

  ~Sphere() = default;

  bool hits(const Raytracer::Ray &ray) const;
};

} // namespace Raytracer
