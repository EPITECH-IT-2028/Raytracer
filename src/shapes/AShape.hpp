#pragma once

#include "IShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

constexpr float EPS = 1e-6;

namespace Raytracer {

  /**
   * @brief Abstract base class for all shapes in the Raytracer.
   *
   * This class implements the IShape interface and provides common
   * properties and methods for shapes, such as center, color, material,
   * shininess, and translation.
   */
  class AShape : public IShape {
    public:
      /**
       * @brief Virtual default destructor for AShape.
       */
      virtual ~AShape() = default;

      /**
       * @brief Pure virtual method to calculate ray-shape intersections.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the shape, and a pointer to the shape itself if hit.
       */
      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override = 0;

      /**
       * @brief Pure virtual method to get the normal vector at a hit point.
       * @param hitPoint The point on the shape's surface.
       * @return The normal vector at the hit point.
       */
      virtual Math::Vector3D getNormal(
          const Math::Point3D &hitPoint) const override = 0;

      /**
       * @brief Translates the shape by a given offset.
       * @param offset The vector by which to translate the shape's center.
       */
      virtual void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      /**
       * @brief Gets the center point of the shape.
       * @return const Math::Point3D& A const reference to the shape's center.
       */
      const Math::Point3D &getCenter() const override {
        return _center;
      }

      /**
       * @brief Gets the color of the shape.
       * @return const Math::Vector3D& A const reference to the shape's color.
       */
      const Math::Vector3D &getColor() const override {
        return _color;
      }

      /**
       * @brief Gets the shininess property of the shape's material.
       * @return double The shininess value.
       */
      double getShininess() const override {
        return _shininess;
      }

      /**
       * @brief Gets the material of the shape.
       * @return std::shared_ptr<IMaterials> A shared pointer to the shape's material.
       */
      std::shared_ptr<IMaterials> getMaterial() const override {
        return _material;
      }

      /**
       * @brief Sets the center point of the shape.
       * @param center The new center point.
       */
      void setCenter(const Math::Point3D &center) override {
        _center = center;
      }

      /**
       * @brief Sets the color of the shape.
       * @param color The new color.
       */
      void setColor(const Math::Vector3D &color) override {
        _color = color;
      }

      /**
       * @brief Sets the shininess property of the shape's material.
       * @param shininess The new shininess value.
       */
      void setShininess(double shininess) override {
        _shininess = shininess;
      }
      /**
       * @brief Sets the material of the shape.
       * @param material A shared pointer to the new material.
       */
      void setMaterial(std::shared_ptr<IMaterials> material) override {
        _material = material;
      }

    protected:
      Math::Point3D _center;                ///< The center point of the shape.
      Math::Vector3D _color;                ///< The color of the shape.
      double _shininess = 32.;              ///< The shininess property for lighting calculations.
      std::shared_ptr<IMaterials> _material; ///< The material of the shape.
  };

}  // namespace Raytracer
