#include "Point3D.hpp"

Math::Point3D Math::Point3D::operator+(const Vector3D &vec) const {
  return Point3D(x + vec.x, y + vec.y, z + vec.z);
}

Math::Vector3D Math::Point3D::operator-(const Point3D &point) const {
  return Vector3D(x - point.x, y - point.y, z - point.z);
}
