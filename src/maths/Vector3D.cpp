#include "Vector3D.hpp"
#include <cmath>

Math::Vector3D Math::Vector3D::operator+(const Vector3D &vec) const {
  return Vector3D(x + vec.x, y + vec.y, z + vec.z);
}

Math::Vector3D Math::Vector3D::operator+=(const Vector3D &vec) {
  x += vec.x;
  y += vec.y;
  z += vec.z;
  return *this;
}

Math::Vector3D Math::Vector3D::operator-(const Vector3D &vec) const {
  return Vector3D(x - vec.x, y - vec.y, z - vec.z);
}

Math::Vector3D Math::Vector3D::operator-=(const Vector3D &vec) {
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;
  return *this;
}

Math::Vector3D Math::Vector3D::operator*(const Vector3D &vec) const {
  return Vector3D(x * vec.x, y * vec.y, z * vec.z);
}

Math::Vector3D Math::Vector3D::operator*=(const Vector3D &vec) {
  x *= vec.x;
  y *= vec.y;
  z *= vec.z;
  return *this;
}

Math::Vector3D Math::Vector3D::operator/(const Vector3D &vec) const {
  return Vector3D(x / vec.x, y / vec.y, z / vec.z);
}

Math::Vector3D Math::Vector3D::operator/=(const Vector3D &vec) {
  x /= vec.x;
  y /= vec.y;
  z /= vec.z;
  return *this;
}

Math::Vector3D Math::Vector3D::operator*(const double &val) const {
  return Vector3D(x * val, y * val, z * val);
}

Math::Vector3D Math::Vector3D::operator*=(const double &val) {
  x *= val;
  y *= val;
  z *= val;
  return *this;
}

Math::Vector3D Math::Vector3D::operator/(const double &val) const {
  return Vector3D(x / val, y / val, z / val);
}

Math::Vector3D Math::Vector3D::operator/=(const double &val) {
  x /= val;
  y /= val;
  z /= val;
  return *this;
}

double Math::Vector3D::dot(const Vector3D &vec) const {
  return x * vec.x + y * vec.y + z * vec.z;
}

Math::Vector3D &Math::Vector3D::normalize() {
  double length = std::sqrt(x * x + y * y + z * z);
  if (length != 0) {
    x /= length;
    y /= length;
    z /= length;
  }
  return *this;
}
