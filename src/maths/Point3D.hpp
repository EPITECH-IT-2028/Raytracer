#pragma once
#include "Vector3D.hpp"

namespace Math {

  class Point3D {
    public:
      float x;
      float y;
      float z;

      Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
      }
      ~Point3D() = default;

      Point3D operator+(const Vector3D &vec) const;
      Math::Vector3D operator-(const Point3D &point) const;
  };

}  // namespace Math
