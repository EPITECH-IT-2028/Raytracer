#pragma once

#include "IMaterials.hpp"

namespace Raytracer {

  class AMaterials : public IMaterials {
    public:
      /**
 * @brief Constructs an AMaterials object.
 *
 * Initializes the abstract material base class for use in ray tracing material computations.
 */
AMaterials() = default;
      /**
 * @brief Virtual destructor for the AMaterials abstract base class.
 *
 * Ensures proper cleanup of derived material classes through base class pointers.
 */
virtual ~AMaterials() = default;

      virtual Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint, const Math::Vector3D &color,
          const Raytracer::ShapeComposite &shapes,
          Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
          int depth, RayColorFunc rayColorFunc) const override = 0;
  };

}  // namespace Raytracer
