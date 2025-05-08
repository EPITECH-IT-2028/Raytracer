#pragma once

#include "IShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class AShape : public IShape {
    public:
      virtual ~AShape() = default;

      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override = 0;

      virtual Math::Vector3D getNormal(
          const Math::Point3D &hitPoint) const override = 0;

      virtual void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      const Math::Point3D &getCenter() const override {
        return _center;
      }
      const Math::Vector3D &getColor() const override {
        return _color;
      }
      double getShininess() const override {
        return _shininess;
      }

      /**
       * @brief Sets the center point of the shape.
       *
       * @param center The new center position for the shape.
       */
      void setCenter(const Math::Point3D &center) override {
        _center = center;
      }

      /**
       * @brief Sets the color of the shape.
       *
       * @param color The new color vector to assign to the shape.
       */
      void setColor(const Math::Vector3D &color) override {
        _color = color;
      }

      /**
       * @brief Sets the shininess coefficient of the shape.
       *
       * @param shininess The new shininess value to assign.
       */
      void setShininess(double shininess) override {
        _shininess = shininess;
      }

      /**
       * @brief Returns the material associated with the shape.
       *
       * @return std::shared_ptr<IMaterials> Shared pointer to the shape's material.
       */
      std::shared_ptr<IMaterials> getMaterial() const override {
        return _material;
      }

      /**
       * @brief Sets the material associated with the shape.
       *
       * @param material Shared pointer to the material to assign.
       */
      void setMaterial(std::shared_ptr<IMaterials> material) override {
        _material = material;
      }

    protected:
      Math::Point3D _center;
      Math::Vector3D _color;
      double _shininess = 32.;
      std::shared_ptr<IMaterials> _material;
  };

}  // namespace Raytracer
