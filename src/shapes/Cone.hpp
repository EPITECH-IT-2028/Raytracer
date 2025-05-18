#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents a finite cone shape in the 3D scene.
   *
   * This class inherits from AShape and defines a cone with a circular base,
   * an apex, specified by its center (base center), radius (of base), height,
   * axis normal, and color.
   */
  class Cone : public AShape {
    public:
      /**
       * @brief Constructs a Cone object.
       * @param center The center point of the cone's base.
       * @param radius The radius of the cone's base.
       * @param height The height of the cone from base to apex.
       * @param normal The normal vector indicating the orientation of the cone's axis (from base to apex).
       * @param color The color of the cone.
       */
      Cone(const Math::Point3D &center, double radius, double height,
           const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal), _radius(radius), _height(height) {
        _center = center;
        _color = color;
      }

      /**
       * @brief Default constructor for Cone.
       * Initializes a cone with radius 1, height 2, base centered at origin (0,0,0),
       * oriented along the Y-axis, and with red color.
       */
      Cone() : _normal(Math::Vector3D(0, 1, 0)), _radius(1.0), _height(2.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for Cone.
       */
      ~Cone() = default;

      /**
       * @brief Calculates the intersection of a ray with the finite cone (body and base cap).
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the cone, and a pointer to the cone itself if hit, otherwise a very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector at a given point on the cone's surface (body or base).
       * @param point The point on the cone's surface.
       * @return The normalized normal vector at that point.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      /**
       * @brief Sets the normal vector (axis) of the cone.
       * @param normal The new normal vector.
       */
      void setNormal(const Math::Vector3D &normal) {
        _normal = normal;
      }

      /**
       * @brief Sets the radius of the cone's base.
       * @param radius The new radius.
       */
      void setRadius(double radius) {
        _radius = radius;
      }

      /**
       * @brief Sets the height of the cone.
       * @param height The new height.
       */
      void setHeight(double height) {
        _height = height;
      }

    private:
      Math::Vector3D _normal; ///< The normal vector defining the cone's axis (from base to apex).
      double _radius;         ///< The radius of the cone's base.
      double _height;         ///< The height of the cone.
  };

}  // namespace Raytracer
