#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Sphere : public AShape {
    public:
      Sphere(const Math::Point3D &center, double radius,
             const Math::Vector3D &color)
          : _center(center), _color(color), _radius(radius) {
      }

      Sphere()
          : _center(Math::Point3D(0, 0, 0)),
            _color(Math::Vector3D(1, 0, 0)),
            _radius(1) {
      }

      ~Sphere() = default;

      std::tuple<double, Math::Vector3D> hits(
          const Raytracer::Ray &ray) const override;

      Math::Point3D getCenter() const {
        return _center;
      }

      Math::Vector3D getColor() const {
        return _color;
      }

    private:
      Math::Point3D _center;
      Math::Vector3D _color;
      double _radius;
  };

}  // namespace Raytracer
