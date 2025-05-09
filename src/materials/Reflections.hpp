#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  /*
   * @brief Reflections class
   * This class is used to compute the reflection of light on a surface.
   */
  class Reflections : public AMaterials {
    public:
      Reflections() = default;

      ~Reflections() override = default;

      Math::Vector3D computeMaterial(const Math::Vector3D &normal,
                                     const Math::Vector3D &viewDir,
                                     const Math::Point3D &hitPoint,
                                     const Math::Vector3D &color,
                                     const Raytracer::ShapeComposite &shapes,
                                     const Raytracer::LightComposite &lights,
                                     const Raytracer::Camera &camera, int depth,
                                     RayColorFunc rayColorFunc) const override;
  };
}  // namespace Raytracer
