#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"

namespace Raytracer {

  class Plane : public AShape {
    public:
      Plane(const Math::Point3D &center, const Math::Vector3D &normal,
            const Math::Vector3D &color)
          : _center(center), _normal(normal), _color(color) {
      }

      Plane()
          : _center(Math::Point3D()),
            _normal(Math::Vector3D()),
            _color(Math::Vector3D(1, 0, 0)) {
      }

      ~Plane() = default;

      std::tuple<bool, Math::Vector3D> hits(const Ray &ray) const override;

    private:
      Math::Point3D _center;
      Math::Vector3D _normal;
      Math::Vector3D _color;
  };

}  // namespace Raytracer
