#include "DirectionalLight.hpp"
#include <algorithm>
#include <iostream>
#include "Vector3D.hpp"

/**
 * @brief Computes the lighting contribution of this directional light at a given hit point.
 *
 * Calculates diffuse lighting based on the angle between the surface normal and the
 * light direction. It also performs a shadow check by casting a ray from the hit point
 * towards the light source. If an object is hit, the point is considered in shadow,
 * and only a minimal light intensity is applied.
 *
 * @param normal The surface normal at the hit point.
 * @param objectColor The color of the object at the hit point.
 * @param hitPoint The point on the surface where the light is being calculated.
 * @param viewDir The direction from the hit point to the camera (unused for this light type).
 * @param shapes A composite of all shapes in the scene, used for shadow checking.
 * @return Math::Vector3D The calculated color contribution from this light.
 */
Math::Vector3D Raytracer::DirectionalLight::computeLighting(
    const Math::Vector3D &normal, const Math::Vector3D &objectColor,
    const Math::Point3D &hitPoint,
    __attribute__((unused)) const Math::Vector3D &viewDir,
    const ShapeComposite &shapes) const {
  Math::Point3D shadowOrigin = hitPoint + normal * 0.001;
  Raytracer::Ray shadowRay(shadowOrigin, -getDirection().normalize());
  auto [shadow, _, shadowShape] = shapes.hits(shadowRay);
  float lightIntensity;

  if (shadow > 0.0 && shadowShape != nullptr) {
    lightIntensity = 0.1f;
  } else {
    lightIntensity = 0.1f + 0.9f * std::max(0.0, normal.dot(-getDirection()));
  }
  return objectColor * lightIntensity;
}

extern "C" {
/**
 * @brief Factory function to create a new DirectionalLight instance.
 *
 * This function is typically used by a plugin system to instantiate light objects.
 * @return Raytracer::ILight* A pointer to the newly created DirectionalLight, or nullptr on failure.
 */
Raytracer::ILight *addLight() {
  try {
    return new Raytracer::DirectionalLight();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create DirectionalLight: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
