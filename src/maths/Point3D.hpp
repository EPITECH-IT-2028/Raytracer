#pragma once

#include "Vector3D.hpp"

namespace Math {

  /**
   * @brief Represents a point in 3D space with x, y, and z coordinates.
   *
   * This class provides basic operations for points, such as addition/subtraction
   * with vectors, and subtraction between points to get a vector.
   */
  class Point3D {
    public:
      float x; ///< The x-coordinate of the point.
      float y; ///< The y-coordinate of the point.
      float z; ///< The z-coordinate of the point.

      /**
       * @brief Constructs a Point3D object.
       * @param x The initial x-coordinate (default is 0).
       * @param y The initial y-coordinate (default is 0).
       * @param z The initial z-coordinate (default is 0).
       */
      Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
      }
      /**
       * @brief Default destructor.
       */
      ~Point3D() = default;

      /**
       * @brief Adds a vector to this point, resulting in a new point.
       * This represents translating the point by the vector.
       * @param vec The vector to add.
       * @return A new Point3D representing the translated point.
       */
      Point3D operator+(const Vector3D &vec) const;
      /**
       * @brief Subtracts a vector from this point, resulting in a new point.
       * This represents translating the point by the negative of the vector.
       * @param vec The vector to subtract.
       * @return A new Point3D representing the translated point.
       */
      Point3D operator-(const Vector3D &vec) const;
      /**
       * @brief Subtracts another point from this point, resulting in a vector.
       * This vector represents the direction and distance from the other point to this point.
       * @param point The point to subtract.
       * @return A Vector3D from `point` to this point.
       */
      Vector3D operator-(const Point3D &point) const;
  };

}  // namespace Math
