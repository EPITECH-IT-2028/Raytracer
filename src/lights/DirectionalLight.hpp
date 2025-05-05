#pragma once

#include "ILight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class DirectionalLight : public ILight {
    public:
      Math::Vector3D direction;

      DirectionalLight() = default;
      DirectionalLight(Math::Vector3D direction) : direction(direction) {};
      ~DirectionalLight() = default;
      Math::Vector3D computeLighting(
          const Math::Vector3D& normal, const Math::Vector3D& objectColor,
          const Math::Point3D& hitPoint,
          const ShapeComposite& shapes) const override;
  };
}  // namespace Raytracer
