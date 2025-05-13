#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class CylinderInf : public AShape {
    public:
      CylinderInf(const Math::Point3D &center, double radius,
               const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal),
            _radius(radius) {
        _center = center;
        _color = color;
      }

      CylinderInf()
          : _normal(Math::Vector3D(0, 1, 0)),
            _radius(1.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      ~CylinderInf() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      double getRadius() const { return _radius; }

      void setRadius(const double &radius) { _radius = radius; }

    private:
      Math::Vector3D _normal;
      double _radius;
  };

}  // namespace Raytracer
