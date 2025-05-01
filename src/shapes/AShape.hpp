#pragma once

#include "IShape.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class AShape : public IShape {
    public:
      virtual ~AShape() = default;

      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const = 0;

      virtual Math::Vector3D getNormal(const Math::Point3D &hitPoint) const = 0;

      virtual void translate(const Math::Vector3D &offset) {
        _center = _center + offset;
      }

    protected:
      Math::Point3D _center;
      Math::Vector3D _color;
  };

}  // namespace Raytracer
