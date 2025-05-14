#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  /*
   * @brief Refractions class
   * This class is used to compute the refraction of light on a surface.
   */
  class Refractions : public AMaterials {
    public:
      Refractions() = default;

      ~Refractions() override = default;

      float getBaseColorRatio() const { return 0.1f; }

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
