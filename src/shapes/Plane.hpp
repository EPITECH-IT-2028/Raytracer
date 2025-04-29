#pragma once

#include "Point3D.hpp"
#include "Ray.hpp"

namespace Raytracer {

class Plane {
public:
  Math::Point3D center;
  Math::Vector3D normal;

  Plane(const Math::Point3D &center, const Math::Vector3D &normal)
      : center(center), normal(normal) {}

  Plane() : center(Math::Point3D()), normal(Math::Vector3D()) {}
  ~Plane() = default;

  bool hits(const Raytracer::Ray &ray) const;
};

} // namespace Raytracer
