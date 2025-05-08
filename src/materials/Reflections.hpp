#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  class Reflections : public AMaterials {
    public:
      Reflections() = default;
      ~Reflections() override = default;

      virtual Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint,
          const Math::Vector3D &color) const override;

      Math::Vector3D reflect(const Math::Vector3D &incident,
                             const Math::Vector3D &normal) const;
  };
}  // namespace Raytracer
