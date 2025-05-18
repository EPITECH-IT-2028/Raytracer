#pragma once

#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents a ray in 3D space, defined by an origin and a direction.
   *
   * A ray is typically used for ray casting and ray tracing algorithms to determine
   * intersections with objects in a scene. The ray is parameterized as P(t) = origin + t * direction.
   */
  class Ray {
    public:
      Math::Point3D origin;    ///< The origin point of the ray.
      Math::Vector3D direction; ///< The direction vector of the ray (should ideally be normalized).

      /**
       * @brief Constructs a Ray object with a specified origin and direction.
       * @param origin The starting point of the ray.
       * @param direction The direction vector of the ray.
       */
      Ray(const Math::Point3D &origin, const Math::Vector3D &direction)
          : origin(origin), direction(direction) {
      }

      /**
       * @brief Default constructor for Ray.
       * Initializes the ray with origin at (0,0,0) and direction vector (0,0,0).
       * Note: A zero direction vector is generally not valid for a ray.
       */
      Ray() : origin(Math::Point3D()), direction(Math::Vector3D()) {
      }

      /**
       * @brief Calculates a point along the ray at a given parameter t.
       * The point is calculated as P(t) = origin + t * direction.
       * @param t The parameter value.
       * @return Math::Point3D The point on the ray at parameter t.
       */
      Math::Point3D at(double t) const {
        return origin + direction * t;
      }

      /**
       * @brief Default destructor for Ray.
       */
      ~Ray() = default;
  };

}  // namespace Raytracer
