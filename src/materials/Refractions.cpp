#include "Refractions.hpp"
#include "Vector3D.hpp"
#include <cmath>
#include <iostream>

/**
 * @brief Computes the color for a refractive material using Snell's Law.
 *
 * Handles entering and exiting the material by adjusting the normal and refractive indices.
 * If total internal reflection occurs, it computes a purely reflected ray. Otherwise,
 * it computes the refracted ray. The color from the new ray (refracted or reflected)
 * is then blended with the object's base color.
 *
 * @param normal The surface normal at the hit point.
 * @param viewDir The direction from the hit point to the camera.
 * @param hitPoint The point of intersection on the surface.
 * @param color The base color of the refractive material.
 * @param shapes The collection of shapes in the scene.
 * @param lights The collection of lights in the scene.
 * @param camera The camera used for rendering.
 * @param depth The current recursion depth for ray tracing.
 * @param rayColorFunc A function to compute the color of a ray.
 * @return Math::Vector3D The blended color resulting from refraction/reflection.
 */
Math::Vector3D Raytracer::Refractions::computeMaterial(
    const Math::Vector3D &normal, const Math::Vector3D &viewDir,
    const Math::Point3D &hitPoint, const Math::Vector3D &color,
    const Raytracer::ShapeComposite &shapes,
    const Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
    int depth, RayColorFunc rayColorFunc) const {
  if (depth <= 0) {
      return color;
  }
  const float n1 = 1.0f;
  const float n2 = 1.5f;
  Math::Vector3D incidentDir = -viewDir;
  incidentDir.normalize();
  float cosI_initial = incidentDir.dot(normal.normalized());
  Math::Vector3D refractiveNormal;
  float refractionRatio;
  if (cosI_initial < 0) {
      refractiveNormal = normal;
      refractionRatio = n1 / n2;
  } else {
      refractiveNormal = -normal;
      refractionRatio = n2 / n1;
  }
  float cosI = incidentDir.dot(refractiveNormal);
  float cosT2 = 1.0f - (refractionRatio * refractionRatio) * (1.0f - cosI * cosI);
  if (cosT2 < 0.0f) {
      Math::Vector3D reflectedDir = incidentDir - refractiveNormal * 2.0f * cosI;
      reflectedDir.normalize();
      Raytracer::Ray reflectedRay(hitPoint + reflectedDir * 0.001f, reflectedDir);
      return rayColorFunc(reflectedRay, shapes, lights, camera, depth - 1);
  }
  float cosT = std::sqrt(cosT2);
  Math::Vector3D refractedDir = incidentDir * refractionRatio + refractiveNormal * (refractionRatio * cosI - cosT);
  refractedDir.normalize();
  Raytracer::Ray refractedRay(hitPoint + refractedDir * 0.001f, refractedDir);
  Math::Vector3D refractedColor = rayColorFunc(refractedRay, shapes, lights, camera, depth - 1);
  const float baseColorRatio = getBaseColorRatio();
  return color * baseColorRatio + refractedColor * (1.0f - baseColorRatio);
}

extern "C" {
/**
 * @brief Factory function to create a new Refractions material instance.
 *
 * This function is typically used by a plugin system to instantiate material objects.
 * @return Raytracer::IMaterials* A pointer to the newly created Refractions material, or nullptr on failure.
 */
Raytracer::IMaterials *addMaterial() {
  try {
    return new Raytracer::Refractions();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Refractions material: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
