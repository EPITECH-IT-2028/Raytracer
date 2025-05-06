#pragma once

#include "ALight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class AmbientLight : public ALight {
    public:
      AmbientLight() = default;
      ~AmbientLight() = default;

      Math::Vector3D computeLighting(const Math::Vector3D& normal,
                                             const Math::Vector3D& objectColor,
                                             const Math::Point3D& hitPoint,
                                             const Math::Vector3D &viewDir,
                                             const ShapeComposite& shapes) override;
  };
}  // namespace Raytracer
