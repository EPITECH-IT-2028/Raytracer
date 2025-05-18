#pragma once

#include <functional>
#include "Camera.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class Renderer;
  class ShapeComposite;
  class LightComposite;
  class Camera;
  class Ray;

  /**
   * @brief Interface for materials in the Raytracer.
   *
   * Defines the contract for how different materials (e.g., diffuse, reflective,
   * refractive, transparent) compute their appearance based on lighting,
   * view direction, and scene context.
   */
  class IMaterials {
    public:
      /**
       * @brief Virtual destructor.
       */
      virtual ~IMaterials() = default;

      /**
       * @brief Type alias for a function that computes the color of a ray.
       *
       * This function is used by materials that need to cast secondary rays (e.g., for
       * reflection, refraction) to determine the color contribution from those rays.
       * It takes a ray, the scene's shapes and lights, the camera, and the current
       * recursion depth as parameters, returning the calculated color.
       * The use of std::function allows passing a callable (like a lambda or member function)
       * that encapsulates the core ray coloring logic, often from the Renderer.
       */
      using RayColorFunc = std::function<Math::Vector3D(
          Raytracer::Ray & /*ray*/,             // The ray to trace
          const Raytracer::ShapeComposite & /*shapes*/, // Scene shapes
          const Raytracer::LightComposite & /*lights*/, // Scene lights
          const Raytracer::Camera & /*camera*/,         // Scene camera
          int /*depth*/                       // Current recursion depth
          )>;

      /**
       * @brief Computes the visual appearance (color) of the material at a given surface point.
       *
       * This is the core method for any material. It determines the material's color
       * based on various factors like surface normal, viewing direction, the point itself,
       * its base color, and interactions with scene lights and other objects (via rayColorFunc
       * for effects like reflection/refraction).
       *
       * @param normal The surface normal vector at the hit point.
       * @param viewDir The direction vector from the hit point towards the camera (viewer).
       * @param hitPoint The 3D coordinates of the point on the surface being shaded.
       * @param color The intrinsic base color of the material/surface.
       * @param shapes A composite object containing all shapes in the scene, for tracing secondary rays.
       * @param lights A composite object containing all light sources in the scene, for lighting calculations.
       * @param camera The scene's camera, potentially useful for view-dependent effects.
       * @param depth The current recursion depth for ray tracing (e.g., for reflections/refractions).
       * @param rayColorFunc A callback function (typically from the Renderer) to compute the color of secondary rays.
       * @return Math::Vector3D The computed color of the material at the hit point.
       */
      virtual Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint, const Math::Vector3D &color,
          const Raytracer::ShapeComposite &shapes,
          const Raytracer::LightComposite &lights,
          const Raytracer::Camera &camera, int depth,
          RayColorFunc rayColorFunc) const = 0;
  };
}  // namespace Raytracer
