#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"

namespace Raytracer {

  // WARNING: The plane is buggy
  class Plane : public AShape {
    public:
      Plane(const std::string &axis, double position,
            const Math::Vector3D &color);

      Plane()
          : _center(Math::Point3D()),
            _normal(Math::Vector3D(0, 1, 0)),
            _color(Math::Vector3D(1, 0, 0)) {
      }

      ~Plane() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return _normal;
      }

    private:
      Math::Point3D _center;
      Math::Vector3D _normal;
      Math::Vector3D _color;
  };

}  // namespace Raytracer
