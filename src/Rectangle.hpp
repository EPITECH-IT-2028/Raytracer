#pragma once

#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents a 3D rectangle defined by an origin and two side vectors.
   *
   * This class can be used to define a rectangular area in 3D space,
   * for example, as a screen plane for a camera or a simple rectangular light source.
   */
  class Rectangle3D {
    public:
      /**
       * @brief Constructs a Rectangle3D object.
       * @param origin The origin point of the rectangle (e.g., bottom-left corner).
       * @param bottom_side A vector representing the direction and length of the bottom side.
       * @param left_side A vector representing the direction and length of the left side.
       *                  Typically, bottom_side and left_side are perpendicular.
       */
      Rectangle3D(const Math::Point3D &origin,
                  const Math::Vector3D &bottom_side,
                  const Math::Vector3D &left_side)
          : _origin(origin), _bottom_side(bottom_side), _left_side(left_side) {
      }

      /**
       * @brief Default constructor for Rectangle3D.
       * Initializes a 2x2 rectangle in the XY plane, centered around (-0,-0,-1) if origin is (-1,-1,-1).
       * More precisely, it creates a rectangle from (-1,-1,-1) to (1,1,-1) if left_side was (0,2,0) and bottom_side was (2,0,0).
       * The default values define a rectangle from P0=(-1,-1,-1) (origin)
       * to P1=P0+bottom_side = (1,-1,-1)
       * to P2=P0+left_side = (-1,1,-1)
       * to P3=P0+bottom_side+left_side = (1,1,-1).
       */
      Rectangle3D()
          : _origin(-1, -1, -1), _bottom_side(2, 0, 0), _left_side(0, 2, 0) {
      }

      /**
       * @brief Default destructor for Rectangle3D.
       */
      ~Rectangle3D() = default;

      /**
       * @brief Calculates a point on the rectangle based on parametric coordinates u and v.
       * @param u The parametric coordinate along the bottom_side (typically in [0, 1]).
       * @param v The parametric coordinate along the left_side (typically in [0, 1]).
       * @return Math::Point3D The calculated point on the rectangle.
       */
      Math::Point3D pointAt(double u, double v) const;

    private:
      Math::Point3D _origin;      ///< The origin point of the rectangle.
      Math::Vector3D _bottom_side; ///< Vector defining the bottom side from the origin.
      Math::Vector3D _left_side;   ///< Vector defining the left side from the origin.
  };

}  // namespace Raytracer
