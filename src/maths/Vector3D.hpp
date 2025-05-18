#pragma once

namespace Math {

  /**
   * @brief Represents a 3D vector with x, y, and z components.
   *
   * This class provides common vector operations such as addition, subtraction,
   * scalar multiplication, dot product, cross product, normalization, and length calculation.
   */
  class Vector3D {
    public:
      float x; ///< The x-component of the vector.
      float y; ///< The y-component of the vector.
      float z; ///< The z-component of the vector.

      /**
       * @brief Constructs a Vector3D object.
       * @param x The initial x-component (default is 0).
       * @param y The initial y-component (default is 0).
       * @param z The initial z-component (default is 0).
       */
      Vector3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
      }

      /**
       * @brief Default destructor.
       */
      ~Vector3D() = default;

      // Operator overloads for vector operations
      /**
       * @brief Adds another vector to this vector (component-wise).
       * @param vec The vector to add.
       * @return A new Vector3D representing the sum.
       */
      Vector3D operator+(const Vector3D &vec) const;
      /**
       * @brief Adds another vector to this vector in-place (component-wise).
       * @param vec The vector to add.
       * @return A reference to this modified vector.
       */
      Vector3D &operator+=(const Vector3D &vec);
      /**
       * @brief Subtracts another vector from this vector (component-wise).
       * @param vec The vector to subtract.
       * @return A new Vector3D representing the difference.
       */
      Vector3D operator-(const Vector3D &vec) const;
      /**
       * @brief Subtracts another vector from this vector in-place (component-wise).
       * @param vec The vector to subtract.
       * @return A reference to this modified vector.
       */
      Vector3D &operator-=(const Vector3D &vec);
      /**
       * @brief Multiplies this vector by another vector (component-wise).
       * @param vec The vector to multiply by.
       * @return A new Vector3D representing the component-wise product.
       */
      Vector3D operator*(const Vector3D &vec) const;
      /**
       * @brief Multiplies this vector by another vector in-place (component-wise).
       * @param vec The vector to multiply by.
       * @return A reference to this modified vector.
       */
      Vector3D &operator*=(const Vector3D &vec);
      /**
       * @brief Divides this vector by another vector (component-wise).
       * @param vec The vector to divide by.
       * @return A new Vector3D representing the component-wise division.
       * @throw std::runtime_error if any component of vec is zero.
       */
      Vector3D operator/(const Vector3D &vec) const;
      /**
       * @brief Divides this vector by another vector in-place (component-wise).
       * @param vec The vector to divide by.
       * @return A reference to this modified vector.
       * @throw std::runtime_error if any component of vec is zero.
       */
      Vector3D &operator/=(const Vector3D &vec);

      // Operator overloads for scalar operations
      /**
       * @brief Multiplies this vector by a scalar value.
       * @param val The scalar value.
       * @return A new Vector3D representing the scaled vector.
       */
      Vector3D operator*(const double &val) const;
      /**
       * @brief Multiplies this vector by a scalar value in-place.
       * @param val The scalar value.
       * @return A reference to this modified vector.
       */
      Vector3D &operator*=(const double &val);
      /**
       * @brief Divides this vector by a scalar value.
       * @param val The scalar value.
       * @return A new Vector3D representing the scaled vector.
       * @throw std::runtime_error if val is zero.
       */
      Vector3D operator/(const double &val) const;
      /**
       * @brief Divides this vector by a scalar value in-place.
       * @param val The scalar value.
       * @return A reference to this modified vector.
       * @throw std::runtime_error if val is zero.
       */
      Vector3D &operator/=(const double &val);
      /**
       * @brief Negates this vector (reverses its direction).
       * @return A new Vector3D representing the negated vector.
       */
      Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
      }

      /**
       * @brief Calculates the dot product of this vector with another vector.
       * @param vec The other vector.
       * @return The dot product.
       */
      double dot(const Vector3D &vec) const;

      /**
       * @brief Normalizes this vector in-place (scales it to unit length).
       * If the vector is a zero vector, it remains unchanged.
       * @return A reference to this modified (normalized) vector.
       */
      Vector3D &normalize();
      /**
       * @brief Returns a normalized version of this vector (unit length).
       * If the vector is a zero vector, a copy of the zero vector is returned.
       * @return A new Vector3D representing the normalized vector.
       */
      Vector3D normalized() const;

      /**
       * @brief Calculates the length (magnitude) of this vector.
       * @return The length of the vector.
       */
      double length() const;

      /**
       * @brief Calculates the squared length (magnitude squared) of this vector.
       * Useful for comparisons as it avoids a square root operation.
       * @return The squared length of the vector.
       */
      double lengthSquared() const { return x * x + y * y + z * z; }
  };

  /**
   * @brief Calculates the cross product of two Vector3D objects.
   * @param A The first vector.
   * @param B The second vector.
   * @return A new Vector3D representing the cross product (A x B).
   */
  Vector3D cross(const Vector3D &A, const Vector3D &B);

}  // namespace Math
