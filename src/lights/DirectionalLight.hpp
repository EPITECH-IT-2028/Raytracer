#pragma once

#include "ALight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class DirectionalLight : public ALight {
    public:
      DirectionalLight() = default;
      DirectionalLight(Math::Vector3D direction) {
        setDirection(direction);
        setType("DirectionalLight");
      };
      ~DirectionalLight() = default;

      const Math::Vector3D reflect(const Math::Vector3D &light, const Math::Vector3D &normal) const;

      Math::Vector3D computeLighting(
        const Math::Vector3D &normal,
        const Math::Vector3D &objectColor,
        const Math::Point3D &hitPoint,
        const Math::Vector3D &viewDir,
        const ShapeComposite &shapes) override;
    private:
      const std::string _type = "DirectionalLight";
  };
}  // namespace Raytracer
