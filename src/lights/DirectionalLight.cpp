#include "DirectionalLight.hpp"
#include <cmath>
#include "Vector3D.hpp"

/**
 * @brief Computes the color contribution of a directional light at a surface point, accounting for shadows.
 *
 * Calculates the lighting effect of a directional light on a surface by casting a shadow ray to determine occlusion. If the point is in shadow, only ambient light is applied; otherwise, diffuse lighting based on the angle between the surface normal and light direction is used.
 *
 * @param normal Surface normal at the hit point.
 * @param objectColor Base color of the object at the hit point.
 * @param hitPoint Position on the surface being shaded.
 * @param shapes Collection of scene shapes used for shadow testing.
 * @return The resulting color at the hit point after applying lighting and shadow effects.
 */
Math::Vector3D Raytracer::DirectionalLight::computeLighting(
    const Math::Vector3D &normal, const Math::Vector3D &objectColor,
    const Math::Point3D &hitPoint,
    __attribute__((unused)) const Math::Vector3D &viewDir,
    const ShapeComposite &shapes) {
  Math::Point3D shadowOrigin = hitPoint + normal * 0.001;
  Raytracer::Ray shadowRay(shadowOrigin, -getDirection().normalize());
  auto [shadow, _, shadowShape] = shapes.hits(shadowRay);
  float lightIntensity;

  if (shadow > 0.0) {
    lightIntensity = 0.1;
  } else {
    lightIntensity = 0.1 + 0.9 * std::max(0.0, normal.dot(-getDirection()));
  }
  return objectColor * lightIntensity;
}

extern "C" {
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
