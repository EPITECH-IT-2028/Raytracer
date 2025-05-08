#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  class Reflections : public AMaterials {
    public:
      /**
 * @brief Constructs a Reflections material instance with default settings.
 */
Reflections() = default;
      /**
 * @brief Destroys the Reflections material instance.
 *
 * Ensures proper cleanup of resources when a Reflections object is deleted.
 */
~Reflections() override = default;

      Math::Vector3D computeMaterial(const Math::Vector3D &normal,
                                     const Math::Vector3D &viewDir,
                                     const Math::Point3D &hitPoint,
                                     const Math::Vector3D &color,
                                     const Raytracer::ShapeComposite &shapes,
                                     Raytracer::LightComposite &lights,
                                     const Raytracer::Camera &camera, int depth,
                                     RayColorFunc rayColorFunc) const override;
  };
}  // namespace Raytracer
