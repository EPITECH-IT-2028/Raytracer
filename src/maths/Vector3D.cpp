#include "Vector3D.hpp"
#include <cmath>
#include <stdexcept>

/**
 * @brief Adds another vector to this vector (component-wise).
 * @param vec The vector to add.
 * @return A new Vector3D representing the sum.
 */
Math::Vector3D Math::Vector3D::operator+(const Vector3D &vec) const {
  return Vector3D(x + vec.x, y + vec.y, z + vec.z);
}

/**
 * @brief Adds another vector to this vector in-place (component-wise).
 * @param vec The vector to add.
 * @return A reference to this modified vector.
 */
Math::Vector3D &Math::Vector3D::operator+=(const Vector3D &vec) {
  x += vec.x;
  y += vec.y;
  z += vec.z;
  return *this;
}

/**
 * @brief Subtracts another vector from this vector (component-wise).
 * @param vec The vector to subtract.
 * @return A new Vector3D representing the difference.
 */
Math::Vector3D Math::Vector3D::operator-(const Vector3D &vec) const {
  return Vector3D(x - vec.x, y - vec.y, z - vec.z);
}

/**
 * @brief Subtracts another vector from this vector in-place (component-wise).
 * @param vec The vector to subtract.
 * @return A reference to this modified vector.
 */
Math::Vector3D &Math::Vector3D::operator-=(const Vector3D &vec) {
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;
  return *this;
}

/**
 * @brief Multiplies this vector by another vector (component-wise).
 * This is also known as the Hadamard product.
 * @param vec The vector to multiply by.
 * @return A new Vector3D representing the component-wise product.
 */
Math::Vector3D Math::Vector3D::operator*(const Vector3D &vec) const {
  return Vector3D(x * vec.x, y * vec.y, z * vec.z);
}

/**
 * @brief Multiplies this vector by another vector in-place (component-wise).
 * This is also known as the Hadamard product.
 * @param vec The vector to multiply by.
 * @return A reference to this modified vector.
 */
Math::Vector3D &Math::Vector3D::operator*=(const Vector3D &vec) {
  x *= vec.x;
  y *= vec.y;
  z *= vec.z;
  return *this;
}

/**
 * @brief Divides this vector by another vector (component-wise).
 * @param vec The vector to divide by. Each component of `vec` must be non-zero.
 * @return A new Vector3D representing the component-wise division.
 * @throws std::runtime_error if any component of `vec` is zero.
 */
Math::Vector3D Math::Vector3D::operator/(const Vector3D &vec) const {
  if (vec.x == 0 || vec.y == 0 || vec.z == 0) {
    throw std::runtime_error("Division by zero");
  }
  return Vector3D(x / vec.x, y / vec.y, z / vec.z);
}

/**
 * @brief Divides this vector by another vector in-place (component-wise).
 * @param vec The vector to divide by. Each component of `vec` must be non-zero.
 * @return A reference to this modified vector.
 * @throws std::runtime_error if any component of `vec` is zero.
 */
Math::Vector3D &Math::Vector3D::operator/=(const Vector3D &vec) {
  if (vec.x == 0 || vec.y == 0 || vec.z == 0) {
    throw std::runtime_error("Division by zero");
  }
  x /= vec.x;
  y /= vec.y;
  z /= vec.z;
  return *this;
}

/**
 * @brief Multiplies this vector by a scalar value.
 * @param val The scalar value to multiply each component by.
 * @return A new Vector3D representing the scaled vector.
 */
Math::Vector3D Math::Vector3D::operator*(const double &val) const {
  return Vector3D(x * val, y * val, z * val);
}

/**
 * @brief Multiplies this vector by a scalar value in-place.
 * @param val The scalar value to multiply each component by.
 * @return A reference to this modified vector.
 */
Math::Vector3D &Math::Vector3D::operator*=(const double &val) {
  x *= val;
  y *= val;
  z *= val;
  return *this;
}

/**
 * @brief Divides this vector by a scalar value.
 * @param val The scalar value to divide each component by. Must be non-zero.
 * @return A new Vector3D representing the scaled vector.
 * @throws std::runtime_error if `val` is zero.
 */
Math::Vector3D Math::Vector3D::operator/(const double &val) const {
  if (val == 0) {
    throw std::runtime_error("Division by zero");
  }
  return Vector3D(x / val, y / val, z / val);
}

/**
 * @brief Divides this vector by a scalar value in-place.
 * @param val The scalar value to divide each component by. Must be non-zero.
 * @return A reference to this modified vector.
 * @throws std::runtime_error if `val` is zero.
 */
Math::Vector3D &Math::Vector3D::operator/=(const double &val) {
  if (val == 0) {
    throw std::runtime_error("Division by zero");
  }
  x /= val;
  y /= val;
  z /= val;
  return *this;
}

/**
 * @brief Calculates the dot product of this vector with another vector.
 * The dot product is a scalar value equal to `x1*x2 + y1*y2 + z1*z2`.
 * @param vec The other vector.
 * @return The dot product as a double.
 */
double Math::Vector3D::dot(const Vector3D &vec) const {
  return x * vec.x + y * vec.y + z * vec.z;
}

/**
 * @brief Calculates the length (magnitude) of this vector.
 * The length is sqrt(x*x + y*y + z*z).
 * @return The length of the vector as a double.
 */
double Math::Vector3D::length() const {
  double length = std::sqrt(x * x + y * y + z * z);
  return length;
}

/**
 * @brief Normalizes this vector in-place, scaling it to unit length.
 * If the vector's length is zero, it remains unchanged.
 * @return A reference to this (now normalized) vector.
 */
Math::Vector3D &Math::Vector3D::normalize() {
  double length = this->length();
  if (length != 0) {
    x /= length;
    y /= length;
    z /= length;
  }
  return *this;
}

/**
 * @brief Returns a normalized version of this vector (unit length).
 * Does not modify the original vector. If the original vector's length is zero,
 * a copy of the zero vector is returned.
 * @return A new Vector3D that is the normalized version of this vector.
 */
Math::Vector3D Math::Vector3D::normalized() const {
  double len = this->length();
  if (len != 0) {
    return Vector3D(x / len, y / len, z / len);
  }
  return *this;
}

/**
 * @brief Calculates the cross product of two 3D vectors.
 * The cross product results in a vector that is perpendicular to both input vectors.
 * @param A The first vector.
 * @param B The second vector.
 * @return A new Vector3D representing the cross product (A x B).
 */
Math::Vector3D Math::cross(const Math::Vector3D &A, const Math::Vector3D &B) {
  return {A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x};
}
