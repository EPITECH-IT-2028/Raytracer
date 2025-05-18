#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  /**
   * @brief Represents a transparent material.
   *
   * This class implements a simple transparency effect by blending the object's
   * base color with the color of whatever is behind it, traced by a new ray.
   */
  class Transparency : public AMaterials {
    public:
      /**
       * @brief Default constructor for Transparency material.
       */
      Transparency() = default;

      /**
       * @brief Default destructor for Transparency material.
       */
      ~Transparency() override = default;

      /**
       * @brief Computes the color for a transparent material.
       *
       * It calculates the color by tracing a new ray in the view direction
       * from the hit point (slightly offset) to find the color of the object behind.
       * This color is then blended with the material's base color based on a
       * fixed transparency factor.
       *
       * @param normal The normal vector at the hit point (unused in this simple implementation).
       * @param viewDir The direction from the hit point towards the camera.
       * @param hitPoint The point of intersection on the surface.
       * @param color The base color of the transparent material.
       * @param shapes The composite of all shapes in the scene.
       * @param lights The composite of all lights in the scene.
       * @param camera The scene camera.
       * @param depth The current recursion depth for ray tracing.
       * @param rayColorFunc A callback function to compute the color of a new ray.
       * @return Math::Vector3D The final computed color, blending the material's color with the background color.
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
