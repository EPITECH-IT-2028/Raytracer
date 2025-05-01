#pragma once

#include <iostream>
#include "Ray.hpp"

namespace Raytracer {

  class IShape {
    public:
      virtual ~IShape() = default;

      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const = 0;

      virtual Math::Vector3D getNormal(const Math::Point3D &hitPoint) const = 0;

      virtual void translate(const Math::Vector3D &offset) = 0;
  };

}  // namespace Raytracer
