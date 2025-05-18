#include "Rectangle.hpp"

/**
 * @brief Calculates a point on the surface of the 3D rectangle.
 *
 * The point is determined by parametric coordinates u and v, which scale
 * the `_bottom_side` and `_left_side` vectors respectively.
 * P(u,v) = origin + u * bottom_side + v * left_side.
 *
 * @param u Parameter along the `_bottom_side` vector (typically [0,1]).
 * @param v Parameter along the `_left_side` vector (typically [0,1]).
 * @return Math::Point3D The point on the rectangle corresponding to u and v.
 */
Math::Point3D Raytracer::Rectangle3D::pointAt(double u, double v) const {
  return _origin + (_bottom_side * u) + (_left_side * v);
}
