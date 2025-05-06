#include "DirectionalLight.hpp"
#include <iostream>
#include "Vector3D.hpp"

Math::Vector3D Raytracer::DirectionalLight::computeLighting(
    const Math::Vector3D& normal, const Math::Vector3D& object_color) const {
  double intensity = std::max(0.0, -normal.dot(direction));
  return object_color * intensity;
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
