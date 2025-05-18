#include "Transparency.hpp"
#include <iostream>
#include "Camera.hpp"
#include "LightComposite.hpp"
#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

/**
 * @brief Computes the color for a transparent material.
 *
 * If the recursion depth is not exhausted, it casts a new ray from the hit point
 * in the direction opposite to the view vector (as if looking through the object).
 * The color obtained from this new ray is then blended with the object's own color
 * using a fixed transparency factor.
 *
 * @param normal The surface normal at the hit point (marked as unused).
 * @param viewDir The direction from the hit point to the camera.
 * @param hitPoint The point of intersection on the surface.
 * @param color The base color of the transparent material.
 * @param shapes The collection of shapes in the scene.
 * @param lights The collection of lights in the scene.
 * @param camera The camera used for rendering.
 * @param depth The current recursion depth for ray tracing.
 * @param rayColorFunc A function to compute the color of a ray.
 * @return Math::Vector3D The blended color of the transparent surface.
 */
Math::Vector3D Raytracer::Transparency::computeMaterial(
    __attribute__((unused)) const Math::Vector3D &normal,
    const Math::Vector3D &viewDir, const Math::Point3D &hitPoint,
    const Math::Vector3D &color, const Raytracer::ShapeComposite &shapes,
    const Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
    int depth, RayColorFunc rayColorFunc) const {
  if (depth <= 0)
    return color;

  Raytracer::Ray Ray(hitPoint + -viewDir.normalized() * 0.001f,
                     -viewDir.normalized());
  Math::Vector3D Object = rayColorFunc(Ray, shapes, lights, camera, depth - 1);
  float transparency = 0.7f;
  return color * (1.0f - transparency) + Object * transparency;
}

extern "C" {
/**
 * @brief Factory function to create a new Transparency material instance.
 *
 * This function is typically used by a plugin system to instantiate material objects.
 * @return Raytracer::IMaterials* A pointer to the newly created Transparency material, or nullptr on failure.
 */
Raytracer::IMaterials *addMaterial() {
  try {
    return new Raytracer::Transparency();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Transparent material: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
