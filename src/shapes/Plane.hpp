#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents an infinite plane shape in the 3D scene.
   *
   * This class inherits from AShape and defines a plane by a normal vector
   * and a point on the plane (its center).
   */
  class Plane : public AShape {
    public:
      /**
       * @brief Default constructor for Plane.
       * Initializes a plane with a default normal (0,0,0), center at origin, and red color.
       * Note: A zero normal is generally not valid for a plane. It should be set explicitly.
       */
      Plane() : _normal(Math::Vector3D()) {
        _center = Math::Point3D();
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for Plane.
       */
      ~Plane() = default;

      /**
       * @brief Calculates the intersection of a ray with the plane.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the plane, and a pointer to the plane itself if hit, otherwise a zero distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Ray &ray) const override;

      /**
       * @brief Gets the normal vector of the plane.
       * @param point The point on the plane's surface (unused, as plane has a constant normal).
       * @return The normal vector of the plane.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return _normal;
      }

      /**
       * @brief Sets the normal vector of the plane.
       * @param normal The new normal vector.
       */
      void setNormal(const Math::Vector3D &normal) {
        _normal = normal;
      }

    private:
      Math::Vector3D _normal; ///< The normal vector defining the plane's orientation.
  };

}  // namespace Raytracer
