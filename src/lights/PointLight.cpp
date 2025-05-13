#include "PointLight.hpp"
#include "Vector3D.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

Math::Vector3D Raytracer::PointLight::computeLighting(
    const Math::Vector3D &normal,
    const Math::Vector3D &objectColor,
    const Math::Point3D &hitPoint,
    __attribute__((unused))const Math::Vector3D &viewDir,
    const Raytracer::ShapeComposite &shapes) const {
  Math::Vector3D lightDir = (_position - hitPoint).normalize();
  Math::Point3D shadowOrigin = hitPoint + normal * 0.001;
  Raytracer::Ray shadowRay(shadowOrigin, -getDirection().normalize());
  auto [shadow, _, shadowShape] = shapes.hits(shadowRay);
  double lightIntensity;

  if (shadow > 0.0 && shadowShape != nullptr) {
    lightIntensity = 0.1f * getIntensity();
  } else {
    lightIntensity = (0.1f + 0.9f * std::max(0.0, normal.dot(lightDir))) * getIntensity();
  }
  return objectColor * lightIntensity * _color;
}

extern "C" {
Raytracer::ILight *addLight() {
  try {
    return new Raytracer::PointLight();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create PointLight: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
