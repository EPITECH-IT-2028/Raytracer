#pragma once

#include "AMaterials.hpp"

namespace Raytracer {

  /**
   * @brief Represents a refractive material (e.g., glass, water).
   *
   * This class implements Snell's law to calculate the direction of a refracted ray.
   * It also handles total internal reflection, in which case a reflected ray is computed instead.
   * The final color is a blend of the material's base color and the color obtained from
   * the refracted/reflected ray.
   */
  class Refractions : public AMaterials {
    public:
      /**
       * @brief Default constructor for Refractions material.
       */
      Refractions() = default;

      /**
       * @brief Default destructor for Refractions material.
       */
      ~Refractions() override = default;

      /**
       * @brief Gets the ratio of the base color mixed into the final refracted color.
       * @return float The base color ratio (e.g., 0.1f means 10% base color, 90% refracted/reflected color).
       */
      float getBaseColorRatio() const { return 0.1f; }

      /**
       * @brief Computes the color for a refractive material.
       *
       * Calculates the refracted ray direction using Snell's Law. If total internal
       * reflection occurs, it computes a reflected ray instead. A new ray is traced
       * in the calculated direction, and its color is blended with the material's
       * base color.
       *
       * @param normal The normal vector at the hit point.
       * @param viewDir The direction from the hit point towards the camera.
       * @param hitPoint The point of intersection on the surface.
       * @param color The base color of the refractive material.
       * @param shapes The composite of all shapes in the scene.
       * @param lights The composite of all lights in the scene.
       * @param camera The scene camera.
       * @param depth The current recursion depth for ray tracing.
       * @param rayColorFunc A callback function to compute the color of a new ray.
       * @return Math::Vector3D The final computed color.
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
