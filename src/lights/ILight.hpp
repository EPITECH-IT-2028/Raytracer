#pragma once

#include <iostream>
#include "Ray.hpp"
#include "ShapeComposite.hpp"

namespace Raytracer {
  class ILight {
    public:
      virtual ~ILight() = default;
      virtual Math::Vector3D computeLighting(const Math::Vector3D& normal,
                                             const Math::Vector3D& objectColor,
                                             const Math::Point3D& hitPoint,
                                             const ShapeComposite& shapes) = 0;
  };
}  // namespace Raytracer
