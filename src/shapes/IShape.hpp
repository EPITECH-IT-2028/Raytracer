#pragma once

#include <memory>
#include "Ray.hpp"
#include "materials/IMaterials.hpp"

namespace Raytracer {

  /**
   * @brief Interface for all drawable shapes in the Raytracer.
   *
   * Defines the common contract for shapes, including methods for
   * ray intersection testing, normal calculation, transformations,
   * and accessing properties like center, color, and material.
   */
  class IShape {
    public:
      /**
       * @brief Virtual destructor.
       */
      virtual ~IShape() = default;

      /**
       * @brief Calculates the intersection of a ray with the shape.
       * @param ray The ray to test for intersection.
       * @return A tuple containing:
       *         - double: The distance from the ray's origin to the intersection point (t).
       *         - Math::Vector3D: The color of the shape at the intersection point.
       *         - const IShape*: A pointer to the shape itself if an intersection occurs.
       *         Implementations should return a t value <= 0 or a null shape pointer if no hit.
       */
      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const = 0;

      /**
       * @brief Gets the normal vector at a specific point on the shape's surface.
       * @param hitPoint The point on the shape's surface for which to calculate the normal.
       * @return Math::Vector3D The normal vector at the hitPoint.
       */
      virtual Math::Vector3D getNormal(const Math::Point3D &hitPoint) const = 0;

      /**
       * @brief Translates the shape by a given offset.
       * @param offset The vector representing the translation.
       */
      virtual void translate(const Math::Vector3D &offset) = 0;

      /**
       * @brief Gets the center point of the shape.
       * @return const Math::Point3D& A const reference to the shape's center.
       */
      virtual const Math::Point3D &getCenter() const = 0;
      /**
       * @brief Gets the base color of the shape.
       * @return const Math::Vector3D& A const reference to the shape's color.
       */
      virtual const Math::Vector3D &getColor() const = 0;
      /**
       * @brief Gets the shininess property of the shape's material.
       * @return double The shininess value.
       */
      virtual double getShininess() const = 0;
      /**
       * @brief Gets the material of the shape.
       * @return std::shared_ptr<IMaterials> A shared pointer to the shape's material.
       */
      virtual std::shared_ptr<IMaterials> getMaterial() const = 0;

      /**
       * @brief Sets the center point of the shape.
       * @param center The new center point.
       */
      virtual void setCenter(const Math::Point3D &center) = 0;
      /**
       * @brief Sets the base color of the shape.
       * @param color The new color.
       */
      virtual void setColor(const Math::Vector3D &color) = 0;
      /**
       * @brief Sets the shininess property of the shape's material.
       * @param shininess The new shininess value.
       */
      virtual void setShininess(double shininess) = 0;
      /**
       * @brief Sets the material of the shape.
       * @param material A shared pointer to the new material.
       */
      virtual void setMaterial(std::shared_ptr<IMaterials> material) = 0;
  };

}  // namespace Raytracer
