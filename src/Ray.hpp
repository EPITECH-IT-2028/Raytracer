#pragma once

#include "Point3D.hpp"
namespace Raytracer {

class Ray {
public:
  Math::Point3D origin;
  Math::Vector3D direction;

  Ray(const Math::Point3D &origin, const Math::Vector3D &direction)
      : origin(origin), direction(direction) {}

  Ray() : origin(Math::Point3D()), direction(Math::Vector3D()) {}

  ~Ray() = default;
};

} // namespace Raytracer
