#pragma once

#include "IMaterials.hpp"

namespace Raytracer {

  class AMaterials : public IMaterials {
    public:
      AMaterials() = default;
      virtual ~AMaterials() = default;

      Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint,
          const Math::Vector3D &color) const override = 0;
  };

}  // namespace Raytracer
