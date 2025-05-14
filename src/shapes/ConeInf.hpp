#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class ConeInf : public AShape {
    public:
      ConeInf(const Math::Point3D &center, double angle,
           const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal), _angle(angle) {
        _center = center;
        _color = color;
      }

      ConeInf() : _normal(Math::Vector3D(0, 1, 0)), _angle(1.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      ~ConeInf() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override;

      void setNormal(const Math::Vector3D &normal) {
        _normal = normal;
      }
      void setAngle(double angle) {
        _angle = angle;
      }

    private:
      Math::Vector3D _normal;
      double _angle;

  };

}  // namespace Raytracer
