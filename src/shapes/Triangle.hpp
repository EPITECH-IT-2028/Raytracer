#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief Represents a triangle shape in the 3D scene.
   *
   * This class inherits from AShape and defines a triangular object
   * specified by three vertices (p1, p2, p3), a normal vector, and a color.
   */
  class Triangle : public AShape {
    public:
      /**
       * @brief Constructs a Triangle object.
       * @param center The center point of the triangle (often calculated as the centroid).
       * @param p1 The first vertex of the triangle.
       * @param p2 The second vertex of the triangle.
       * @param p3 The third vertex of the triangle.
       * @param normal The normal vector of the triangle's surface.
       * @param color The color of the triangle.
       */
      Triangle(const Math::Point3D &center, const Math::Point3D &p1,
               const Math::Point3D &p2, const Math::Point3D &p3,
               const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal),
            _p1(p1),
            _p2(p2),
            _p3(p3) {
        _center = center;
        _color = color;
      }

      /**
       * @brief Default constructor for Triangle.
       * Initializes a triangle with default vertices, normal, and red color.
       */
      Triangle()
          : _normal(Math::Vector3D(0, 1, 0)),
            _p1(Math::Point3D(0, 0, 0)),
            _p2(Math::Point3D(1, 0, 0)),
            _p3(Math::Point3D(0, 1, 0)) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      /**
       * @brief Default destructor for Triangle.
       */
      ~Triangle() = default;

      /**
       * @brief Calculates the intersection of a ray with the triangle.
       * Uses the Möller–Trumbore intersection algorithm.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the triangle, and a pointer to the triangle itself if hit, otherwise a zero distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector of the triangle.
       * @param point The point on the triangle's surface (unused, as triangle has a constant normal).
       * @return The normal vector of the triangle.
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return _normal;
      };

      /**
       * @brief Translates the triangle by a given offset.
       * This implementation only translates the center. For full triangle translation,
       * vertices p1, p2, and p3 should also be translated.
       * @param offset The vector by which to translate the triangle's center.
       */
      void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      /**
       * @brief Sets the first vertex of the triangle.
       * @param p1 The new first vertex.
       */
      void setP1(const Math::Point3D &p1) { _p1 = p1; }
      /**
       * @brief Sets the second vertex of the triangle.
       * @param p2 The new second vertex.
       */
      void setP2(const Math::Point3D &p2) { _p2 = p2; }
      /**
       * @brief Sets the third vertex of the triangle.
       * @param p3 The new third vertex.
       */
      void setP3(const Math::Point3D &p3) { _p3 = p3; }

    private:
      Math::Vector3D _normal; ///< The normal vector of the triangle.
      Math::Point3D _p1;      ///< The first vertex of the triangle.
      Math::Point3D _p2;      ///< The second vertex of the triangle.
      Math::Point3D _p3;      ///< The third vertex of the triangle.
  };

}  // namespace Raytracer
