#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Plane : public AShape {
    public:

      Plane() : _normal(Math::Vector3D()) {
        _center = Math::Point3D();
        _color = Math::Vector3D(1, 0, 0);
      }

      ~Plane() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return _normal;
      }

      void setNormal(const Math::Vector3D &normal) {_normal = normal;}
      void setColor(const Math::Vector3D &color) {_color = color;}
      void setCenter(const Math::Point3D &center) {_center = center;}

    private:
      Math::Vector3D _normal;
      Math::Vector3D _color;
      Math::Point3D _center;
  };

}  // namespace Raytracer
