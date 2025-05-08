#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Cylinder : public AShape {
    public:
      Cylinder(const Math::Point3D &center, double radius, double height,
               const Math::Vector3D &normal, const Math::Vector3D &color)
          : _center(center),
            _normal(normal),
            _radius(radius),
            _height(height),
            _color(color) {
      }

      Cylinder()
          : _center(Math::Point3D(0, 0, 0)),
            _normal(Math::Vector3D(0, 1, 0)),
            _radius(1.0),
            _height(2.0),
            _color(Math::Vector3D(0, 0, 1)) {
      }

      ~Cylinder() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      double getRadius() const { return _radius; }
      double getHeight() const { return _height; }

      void setRadius(const double &radius) { _radius = radius; }
      void setHeight(const double &h) { _height = h; }

    private:
      Math::Point3D _center;
      Math::Vector3D _normal;
      double _radius;
      double _height;
      Math::Vector3D _color;
  };

}  // namespace Raytracer
