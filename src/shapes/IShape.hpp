#pragma once

#include <iostream>
#include "Ray.hpp"

namespace Raytracer {

  class IShape {
    public:
      virtual ~IShape() = default;

      virtual std::tuple<bool, Math::Vector3D> hits(
          const Raytracer::Ray &ray) const = 0;
  };

}  // namespace Raytracer
