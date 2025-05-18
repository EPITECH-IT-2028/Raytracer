#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents an infinite double cone shape in the 3D scene.
   *
   * This class inherits from AShape and defines an infinitely long double cone
   * (two cones meeting at their apex) with a center (apex), opening angle,
   * axis normal, and color.
   */
  class ConeInf : public AShape {
    public:
      /**
       * @brief Constructs an infinite Cone object.
       * @param center The apex point of the cone.
       * @param angle The half-angle of the cone in radians.
       * @param normal The normal vector indicating the orientation of the cone's axis.
       * @param color The color of the cone.
       */
      ConeInf(const Math::Point3D &center, double angle,
           const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal), _angle(angle) {
        _center = center;
        _color = color;
      }

      /**
       * @brief Default constructor for ConeInf.
       * Initializes an infinite cone with angle 1.0 radian, apex at origin (0,0,0),
       * oriented along the Y-axis, and with red color.
       */
      ConeInf() : _normal(Math::Vector3D(0, 1, 0)), _angle(1.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for ConeInf.
       */
      ~ConeInf() = default;

      /**
       * @brief Calculates the intersection of a ray with the infinite cone.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the cone, and a pointer to the cone itself if hit, otherwise a very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector at a given point on the cone's surface.
       * @param point The point on the cone's surface.
       * @return The normalized normal vector at that point.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      /**
       * @brief Sets the normal vector (axis) of the infinite cone.
       * @param normal The new normal vector.
       */
      void setNormal(const Math::Vector3D &normal) {
        _normal = normal;
      }

      /**
       * @brief Sets the half-angle of the infinite cone.
       * @param angle The new half-angle in radians.
       */
      void setAngle(double angle) {
        _angle = angle;
      }

      /**
       * @brief Rotates the infinite cone around a specified axis by a given angle.
       * @param axis The axis of rotation.
       * @param angle The angle of rotation in radians.
       */
      void rotate(const Math::Vector3D &axis, float angle) override;

    private:
      Math::Vector3D _normal; ///< The normal vector defining the cone's axis.
      double _angle;          ///< The half-angle of the cone in radians.

  };

}  // namespace Raytracer
