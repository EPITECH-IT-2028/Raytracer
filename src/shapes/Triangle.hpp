#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Triangle : public AShape {
    public:
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

      Triangle()
          : _normal(Math::Vector3D(0, 1, 0)),
            _p1(Math::Point3D(0, 0, 0)),
            _p2(Math::Point3D(1, 0, 0)),
            _p3(Math::Point3D(0, 1, 0)) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      ~Triangle() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return _normal;
      };

      void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      void setP1(const Math::Point3D &p1) { _p1 = p1; }
      void setP2(const Math::Point3D &p2) { _p2 = p2; }
      void setP3(const Math::Point3D &p3) { _p3 = p3; }

    private:
      Math::Vector3D _normal;
      Math::Point3D _p1;
      Math::Point3D _p2;
      Math::Point3D _p3;
  };

}  // namespace Raytracer
