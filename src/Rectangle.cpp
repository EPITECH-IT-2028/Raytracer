#include "Rectangle.hpp"

Math::Point3D Raytracer::Rectangle3D::pointAt(double u, double v) const {
  return origin + (bottom_side * u) + (left_side * v);
}
