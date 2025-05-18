#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  /**
   * @brief Represents a reflective material (e.g., mirror).
   *
   * This class implements perfect reflection. The color of the material is
   * determined by tracing a new ray in the reflection direction and blending
   * its result with the material's base color.
   */
  class Reflections : public AMaterials {
    public:
      /**
       * @brief Default constructor for Reflections material.
       */
      Reflections() = default;

      /**
       * @brief Default destructor for Reflections material.
       */
      ~Reflections() override = default;

      /**
       * @brief Computes the color for a reflective material.
       *
       * Calculates the reflection direction based on the surface normal and view direction.
       * A new ray is traced in this reflection direction, and its color is blended
       * with the material's base color using a fixed reflectivity factor.
       *
       * @param normal The normal vector at the hit point.
       * @param viewDir The direction from the hit point towards the camera.
       * @param hitPoint The point of intersection on the surface.
       * @param color The base color of the reflective material.
       * @param shapes The composite of all shapes in the scene.
       * @param lights The composite of all lights in the scene.
       * @param camera The scene camera.
       * @param depth The current recursion depth for ray tracing.
       * @param rayColorFunc A callback function to compute the color of a new ray.
       * @return Math::Vector3D The final computed color, blending the material's color with the reflected color.
       */
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
