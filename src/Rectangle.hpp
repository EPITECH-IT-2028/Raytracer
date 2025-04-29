#pragma once

#include "Point3D.hpp"

namespace Raytracer {

  class Rectangle3D {
    public:
      Rectangle3D(const Math::Point3D &origin,
                  const Math::Vector3D &bottom_side,
                  const Math::Vector3D &left_side)
          : _origin(origin), _bottom_side(bottom_side), _left_side(left_side) {
      }

      Rectangle3D()
          : _origin(-1, -1, -1), _bottom_side(2, 0, 0), _left_side(0, 2, 0) {
      }

      ~Rectangle3D() = default;

      Math::Point3D pointAt(double u, double v) const;

    private:
      Math::Point3D _origin;
      Math::Vector3D _bottom_side;
      Math::Vector3D _left_side;
  };

}  // namespace Raytracer
