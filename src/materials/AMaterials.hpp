#pragma once

#include "IMaterials.hpp"

namespace Raytracer {

  class AMaterials : public IMaterials {
    public:
      AMaterials() = default;
      virtual ~AMaterials() = default;

      virtual Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint, const Math::Vector3D &color,
          const Raytracer::ShapeComposite &shapes,
          const Raytracer::LightComposite &lights,
          const Raytracer::Camera &camera, int depth,
          RayColorFunc rayColorFunc) const override = 0;
  };

}  // namespace Raytracer
