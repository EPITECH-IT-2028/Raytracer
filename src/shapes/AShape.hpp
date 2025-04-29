#pragma once

#include "IShape.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class AShape : public IShape {
    public:
      virtual ~AShape() = default;

      virtual std::tuple<bool, Math::Vector3D> hits(
          const Raytracer::Ray &ray) const = 0;
  };

}  // namespace Raytracer
