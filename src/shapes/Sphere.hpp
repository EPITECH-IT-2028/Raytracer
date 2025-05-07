#pragma once

#include <stdexcept>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Sphere : public AShape {
    public:
      Sphere(const Math::Point3D &center, double radius,
             const Math::Vector3D &color)
          : _radius(radius) {
        _center = center;
        _color = color;
      }

      Sphere() : _radius(1) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      ~Sphere() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        return (point - _center).normalize();
      }

      const double &getRadius() const {
        return _radius;
      };

      void setCenter(const Math::Point3D& center) { _center = center;}
      void setColor(const Math::Vector3D& color) { _color = color;}
      void setRadius(double radius) {
        if (radius <= 0)
          throw std::runtime_error("Radius must be superior to 0");
        _radius = radius;
      }

    private:
      double _radius;
  };

}  // namespace Raytracer
