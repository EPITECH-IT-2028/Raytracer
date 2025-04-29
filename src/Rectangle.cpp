#include "Rectangle.hpp"

Math::Point3D Raytracer::Rectangle3D::pointAt(double u, double v) const {
  return _origin + (_bottom_side * u) + (_left_side * v);
}
