#pragma once

#include "IMaterials.hpp"

namespace Raytracer {

  /**
   * @brief Abstract base class for materials.
   *
   * This class implements the IMaterials interface and serves as a base
   * for concrete material implementations. It provides default constructor
   * and destructor. The `computeMaterial` method remains pure virtual,
   * requiring derived classes to implement their specific shading logic.
   */
  class AMaterials : public IMaterials {
    public:
      /**
       * @brief Default constructor.
       */
      AMaterials() = default;
      /**
       * @brief Default virtual destructor.
       */
      virtual ~AMaterials() = default;

      /**
       * @brief Pure virtual method to compute the material's color at a hit point.
       *
       * Derived classes must implement this method to define their specific
       * appearance and interaction with light and the scene.
       *
       * @param normal The surface normal vector at the hit point.
       * @param viewDir The direction vector from the hit point towards the camera.
       * @param hitPoint The 3D coordinates of the point on the surface.
       * @param color The intrinsic base color of the material.
       * @param shapes A composite object containing all shapes in the scene.
       * @param lights A composite object containing all light sources in the scene.
       * @param camera The scene's camera.
       * @param depth The current recursion depth for ray tracing.
       * @param rayColorFunc A callback function to compute the color of secondary rays.
       * @return Math::Vector3D The computed color of the material.
       */
      virtual Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint, const Math::Vector3D &color,
          const Raytracer::ShapeComposite &shapes,
          const Raytracer::LightComposite &lights,
          const Raytracer::Camera &camera, int depth,
          RayColorFunc rayColorFunc) const override = 0;
  };

}  // namespace Raytracer
