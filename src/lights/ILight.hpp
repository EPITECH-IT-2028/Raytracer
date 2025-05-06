#pragma once

#include <iostream>
#include <memory>
#include "AShape.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class ILight {
    public:
      virtual ~ILight() = default;
      virtual Math::Vector3D computeLighting(const Math::Vector3D& normal,
                                             const Math::Vector3D& objectColor,
                                             const Math::Point3D& hitPoint,
                                             const Math::Vector3D &viewDir,
                                             const ShapeComposite& shapes) = 0;
      virtual const std::string &getType() const = 0;
  };
}  // namespace Raytracer
