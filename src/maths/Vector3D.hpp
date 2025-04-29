#pragma once

namespace Math {

  class Vector3D {
    public:
      float x;
      float y;
      float z;

      Vector3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
      }

      ~Vector3D() = default;

      // Operator overloads for vector operations
      Vector3D operator+(const Vector3D &vec) const;
      Vector3D &operator+=(const Vector3D &vec);
      Vector3D operator-(const Vector3D &vec) const;
      Vector3D &operator-=(const Vector3D &vec);
      Vector3D operator*(const Vector3D &vec) const;
      Vector3D &operator*=(const Vector3D &vec);
      Vector3D operator/(const Vector3D &vec) const;
      Vector3D &operator/=(const Vector3D &vec);

      // Operator overloads for scalar operations
      Vector3D operator*(const double &val) const;
      Vector3D &operator*=(const double &val);
      Vector3D operator/(const double &val) const;
      Vector3D &operator/=(const double &val);

      double dot(const Vector3D &vec) const;

      Vector3D &normalize();
  };

}  // namespace Math
