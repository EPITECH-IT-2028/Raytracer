#pragma once

#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class ILight {
    public:
      virtual ~ILight() = default;
      virtual Math::Vector3D computeLighting(
          const Math::Vector3D &normal, const Math::Vector3D &objectColor,
          const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
          const ShapeComposite &shapes) const = 0;

      virtual const std::string &getType() const = 0;
      virtual Math::Vector3D getDirection() const = 0;
      virtual const Math::Vector3D &getColor() const = 0;
      virtual double getIntensity() const = 0;
  };
}  // namespace Raytracer
