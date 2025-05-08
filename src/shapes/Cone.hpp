#pragma once

#include <tuple>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Cone : public AShape {
    public:
      Cone(const Math::Point3D &center, double radius, double height,
           const Math::Vector3D &normal, const Math::Vector3D &color)
          : _normal(normal),
            _radius(radius),
            _height(height) {
        _center = center;
        _color = color;
      }

      Cone()
          : _normal(Math::Vector3D(0, 1, 0)),
            _radius(1.0),
            _height(2.0) {
        _center = Math::Point3D(0, 0, 0);
        _color = Math::Vector3D(1, 0, 0);
      }

      ~Cone() = default;

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override;
    
    void setNormal(const Math::Vector3D &normal) { _normal = normal; }
    void setRadius(double radius) { _radius = radius; }
    void setHeight(double height) { _height = height; }

private:
    Math::Vector3D _normal;
    double _radius;
    double _height;
};

}  // namespace Raytracer
