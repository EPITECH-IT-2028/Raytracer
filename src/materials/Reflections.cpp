#include "Reflections.hpp"
#include "Vector3D.hpp"

/**
 * @brief Computes the color contribution of a reflective material at a surface point.
 *
 * Calculates the reflection direction based on the surface normal and view direction, generates a reflected ray, and recursively determines the reflected color using the provided ray color function. The final color is a blend of the base color and the reflected color, weighted 10% and 90% respectively.
 *
 * @param normal Surface normal at the hit point.
 * @param viewDir Direction from the hit point toward the viewer.
 * @param hitPoint Point of intersection on the surface.
 * @param color Base color of the material at the hit point.
 * @param shapes Collection of scene shapes for intersection tests.
 * @param lights Collection of scene lights.
 * @param camera Camera used for rendering.
 * @param depth Current recursion depth for ray tracing.
 * @param rayColorFunc Function to compute the color for a given ray.
 * @return Math::Vector3D The resulting color after combining base and reflected contributions.
 */
Math::Vector3D Raytracer::Reflections::computeMaterial(
    const Math::Vector3D &normal, const Math::Vector3D &viewDir,
    const Math::Point3D &hitPoint, const Math::Vector3D &color,
    const Raytracer::ShapeComposite &shapes, Raytracer::LightComposite &lights,
    const Raytracer::Camera &camera, int depth,
    RayColorFunc rayColorFunc) const {
  Math::Vector3D reflectedDir = -viewDir + normal * (2 * viewDir.dot(normal));
  reflectedDir.normalize();

  Raytracer::Ray reflectedRay(hitPoint + reflectedDir * 0.001, reflectedDir);

  Math::Vector3D reflectedColor =
      rayColorFunc(reflectedRay, shapes, lights, camera, depth);

  return color * (1.0 - 0.9) + reflectedColor * 0.9;
}

extern "C" {
/**
 * @brief Creates and returns a new instance of the Reflections material.
 *
 * @return Pointer to a new Reflections material, or nullptr if creation fails.
 */
Raytracer::IMaterials *addMaterial() {
  try {
    return new Raytracer::Reflections();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Reflections material: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
