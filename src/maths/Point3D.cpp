#include "Point3D.hpp"

/**
 * @brief Translates a point by adding a vector.
 * @param vec The vector to add to the point.
 * @return A new Point3D representing the original point translated by the vector.
 */
Math::Point3D Math::Point3D::operator+(const Vector3D &vec) const {
  return Point3D(x + vec.x, y + vec.y, z + vec.z);
}

/**
 * @brief Translates a point by subtracting a vector.
 * @param vec The vector to subtract from the point.
 * @return A new Point3D representing the original point translated by the negative of the vector.
 */
Math::Point3D Math::Point3D::operator-(const Vector3D &vec) const {
  return Point3D(x - vec.x, y - vec.y, z - vec.z);
}

/**
 * @brief Computes the vector from another point to this point.
 * @param point The starting point of the resulting vector.
 * @return A Vector3D directed from `point` to this point.
 */
Math::Vector3D Math::Point3D::operator-(const Point3D &point) const {
  return Vector3D(x - point.x, y - point.y, z - point.z);
}
