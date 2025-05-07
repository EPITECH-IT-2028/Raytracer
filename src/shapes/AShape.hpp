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

      void setCenter(const Math::Point3D &center) override {
        _center = center;
      }
      void setColor(const Math::Vector3D &color) override {
        _color = color;
      }
      void setShininess(double shininess) override {
        _shininess = shininess;
      }

    protected:
      Math::Point3D _center;
      Math::Vector3D _color;
      double _shininess = 32.;
  };

}  // namespace Raytracer
