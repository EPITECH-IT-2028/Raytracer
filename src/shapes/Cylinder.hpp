#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents a cylinder shape in the 3D scene.
   *
   * This class inherits from AShape and defines a cylindrical object
   * with a center, radius, height, normal vector (orientation), and color.
   */
  class Cylinder : public AShape {
    public:
      /**
       * @brief Constructs a Cylinder object.
       * @param center The center point of the base of the cylinder.
       * @param radius The radius of the cylinder.
       * @param height The height of the cylinder.
       * @param normal The normal vector indicating the orientation of the cylinder's axis.
       * @param color The color of the cylinder.
       */
      Cylinder(const Math::Point3D &center, double radius, double height,
               const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal),
            _radius(radius),
            _height(height) {
        _center = center;
        _color = color;
      }

      /**
       * @brief Default constructor for Cylinder.
       * Initializes a cylinder with radius 1, height 2, centered at origin (0,0,0),
       * oriented along the Y-axis, and with red color.
       */
      Cylinder()
          : _normal(Math::Vector3D(0, 1, 0)),
            _radius(1.0),
            _height(2.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for Cylinder.
       */
      ~Cylinder() = default;

      /**
       * @brief Calculates the intersection of a ray with the cylinder.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the cylinder, and a pointer to the cylinder itself if hit, otherwise a very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector at a given point on the cylinder's surface.
       * @param point The point on the cylinder's surface.
       * @return The normalized normal vector at that point.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      /**
       * @brief Translates the cylinder by a given offset.
       * @param offset The vector by which to translate the cylinder's center.
       */
      void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      /**
       * @brief Gets the radius of the cylinder.
       * @return double The radius of the cylinder.
       */
      double getRadius() const { return _radius; }
      /**
       * @brief Gets the height of the cylinder.
       * @return double The height of the cylinder.
       */
      double getHeight() const { return _height; }

      /**
       * @brief Sets the radius of the cylinder.
       * @param radius The new radius for the cylinder.
       */
      void setRadius(const double &radius) { _radius = radius; }
      /**
       * @brief Sets the height of the cylinder.
       * @param h The new height for the cylinder.
       */
      void setHeight(const double &h) { _height = h; }

    private:
      Math::Vector3D _normal; ///< The normal vector defining the cylinder's axis.
      double _radius;         ///< The radius of the cylinder.
      double _height;         ///< The height of the cylinder.
  };

}  // namespace Raytracer
