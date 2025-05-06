#include "DirectionalLight.hpp"
#include <cmath>
#include "Vector3D.hpp"


Math::Vector3D Raytracer::DirectionalLight::computeLighting(
  const Math::Vector3D &normal,
  const Math::Vector3D &objectColor,
  const Math::Point3D &hitPoint,
  __attribute__((unused))const Math::Vector3D& viewDir,
  const ShapeComposite &shapes
) {
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
  Raytracer::ILight* addLight() {
    try {
      return new Raytracer::DirectionalLight();
    } catch (const std::exception& e) {
      std::cerr << "[ERROR] - Failed to create DirectionalLight: " << e.what() << std::endl;
      return nullptr;
    }
  }
}
