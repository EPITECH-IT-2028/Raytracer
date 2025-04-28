#pragma once

#include "Point3D.hpp"
#include "Ray.hpp"

namespace Raytracer {

class Plane {
public:
  Math::Point3D center;
  Math::Vector3D normal = Math::Vector3D(0, 0, 1);
  double height;
  double width;

  Plane(const Math::Point3D &center, double height, double width)
      : center(center), height(height), width(width) {}
  Plane() : center(Math::Point3D()), height(10), width(10) {}

  ~Plane() = default;

  bool hits(const Raytracer::Ray &ray) const;
};

} // namespace Raytracer
