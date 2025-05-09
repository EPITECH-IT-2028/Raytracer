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

  class IMaterials {
    public:
      virtual ~IMaterials() = default;

      /*
       * @brief Type alias for a function that computes the color of a ray.
       * This function takes a ray, a collection of shapes, a collection of
       * lights, a camera and a recursion depth as parameters and returns a
       * Math::Vector3D representing the color.
       * We need to use a lambda for the circular dependency between IMaterials
       * and Renderer.
       */
      using RayColorFunc = std::function<Math::Vector3D(
          Raytracer::Ray &, const Raytracer::ShapeComposite &,
          const Raytracer::LightComposite &, const Raytracer::Camera &, int)>;

      /*
       * @brief Computes the color of a material at a given hit point.
       * @param normal The normal vector at the hit point.
       * @param viewDir The direction from the hit point to the camera.
       * @param hitPoint The hit point in 3D space.
       * @param color The base color of the material.
       * @param shapes The collection of shapes in the scene.
       * @param lights The collection of lights in the scene.
       * @param camera The camera used for rendering.
       * @param depth The current recursion depth for ray tracing.
       * @param rayColorFunc A function to compute the color of a ray.
       * @return The computed color of the material at the hit point.
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
