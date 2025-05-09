#pragma once

#include "ALight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class DirectionalLight : public ALight {
    public:
      DirectionalLight() = default;
      ~DirectionalLight() = default;

      Math::Vector3D computeLighting(
          const Math::Vector3D &normal, const Math::Vector3D &objectColor,
          const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
          const ShapeComposite &shapes) const override;
  };
}  // namespace Raytracer
