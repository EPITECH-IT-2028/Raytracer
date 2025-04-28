#pragma once

#include "Point3D.hpp"

namespace Raytracer {

class Rectangle3D {
public:
  Math::Point3D origin;
  Math::Vector3D bottom_side;
  Math::Vector3D left_side;

  Rectangle3D(const Math::Point3D &origin, const Math::Vector3D &bottom_side,
              const Math::Vector3D &left_side)
      : origin(origin), bottom_side(bottom_side), left_side(left_side) {}

  Rectangle3D()
      : origin(-1, -1, -1), bottom_side(2, 0, 0), left_side(0, 2, 0) {}

  ~Rectangle3D() = default;

  Math::Point3D pointAt(double u, double v) const;
};

} // namespace Raytracer
