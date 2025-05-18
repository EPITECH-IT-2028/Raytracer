#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents an infinite cylinder shape in the 3D scene.
   *
   * This class inherits from AShape and defines an infinitely long cylindrical
   * object with a center (point on its axis), radius, normal vector (axis
   * orientation), and color.
   */
  class CylinderInf : public AShape {
    public:
      /**
       * @brief Constructs an infinite Cylinder object.
       * @param center A point on the axis of the cylinder.
       * @param radius The radius of the cylinder.
       * @param normal The normal vector indicating the orientation of the
       * cylinder's axis.
       * @param color The color of the cylinder.
       */
      CylinderInf(const Math::Point3D &center, double radius,
                  const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal), _radius(radius) {
        _center = center;
        _color = color;
      }

      /**
       * @brief Default constructor for CylinderInf.
       * Initializes an infinite cylinder with radius 1, centered at origin
       * (0,0,0), oriented along the Y-axis, and with red color.
       */
      CylinderInf() : _normal(Math::Vector3D(0, 1, 0)), _radius(1.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for CylinderInf.
       */
      ~CylinderInf() = default;

      /**
       * @brief Calculates the intersection of a ray with the infinite cylinder.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of
       * the cylinder, and a pointer to the cylinder itself if hit, otherwise a
       * very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector at a given point on the cylinder's
       * surface.
       * @param point The point on the cylinder's surface.
       * @return The normalized normal vector at that point.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      /**
       * @brief Translates the infinite cylinder by a given offset.
       * @param offset The vector by which to translate the cylinder's center
       * point.
       */
      void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      /**
       * @brief Rotates the infinite cylinder around a specified axis by a given
       * angle.
       * @param axis The axis of rotation.
       * @param angle The angle of rotation in radians.
       */
      void rotate(const Math::Vector3D &axis, float angle) override;

      /**
       * @brief Gets the radius of the infinite cylinder.
       * @return double The radius of the cylinder.
       */
      double getRadius() const {
        return _radius;
      }

      /**
       * @brief Sets the radius of the infinite cylinder.
       * @param radius The new radius for the cylinder.
       */
      void setRadius(const double &radius) {
        _radius = radius;
      }

    private:
      Math::Vector3D
          _normal;     ///< The normal vector defining the cylinder's axis.
      double _radius;  ///< The radius of the cylinder.
  };

}  // namespace Raytracer
