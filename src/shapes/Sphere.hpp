#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"
#include "exceptions/RaytracerException.hpp"

namespace Raytracer {

  /**
   * @brief Represents a sphere shape in the 3D scene.
   *
   * This class inherits from AShape and defines a spherical object
   * with a center, radius, and color.
   */
  class Sphere : public AShape {
    public:
      /**
       * @brief Constructs a Sphere object.
       * @param center The center point of the sphere.
       * @param radius The radius of the sphere.
       * @param color The color of the sphere.
       */
      Sphere(const Math::Point3D &center, double radius,
             const Math::Vector3D &color)
          : _radius(radius) {
        _center = center;
        _color = color;
      }

      /**
       * @brief Default constructor for Sphere.
       * Initializes a sphere with radius 1 at origin (0,0,0) and red color.
       */
      Sphere() : _radius(1) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for Sphere.
       */
      ~Sphere() = default;

      /**
       * @brief Calculates the intersection of a ray with the sphere.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the sphere, and a pointer to the sphere itself if hit, otherwise a very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector at a given point on the sphere's surface.
       * @param point The point on the sphere's surface.
       * @return The normalized normal vector at that point.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        return (point - _center).normalize();
      }

      /**
       * @brief Gets the radius of the sphere.
       * @return const double& A const reference to the sphere's radius.
       */
      const double &getRadius() const {
        return _radius;
      };

      /**
       * @brief Sets the radius of the sphere.
       * @param radius The new radius for the sphere.
       * @throws RaytracerError if the radius is not positive.
       */
      void setRadius(double radius) {
        if (radius <= 0) {
          throw RaytracerError("Radius of a sphere must be over 0.");
        }
        _radius = radius;
      }

    private:
      double _radius;
  };

}  // namespace Raytracer
