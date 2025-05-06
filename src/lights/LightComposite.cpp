#include "LightComposite.hpp"
#include <cmath>
#include <memory>
#include <stdexcept>
#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "Vector3D.hpp"

const Math::Vector3D Raytracer::LightComposite::reflect(const Math::Vector3D &incident, const Math::Vector3D &normal) const {
  return incident - (normal * (incident.dot(normal) * 2));
}

Math::Vector3D Raytracer::LightComposite::computeLighting(
  const Math::Vector3D &normal,
  const IShape *hitShape,
  const Math::Vector3D &viewDir
) const {
  if (hitShape == nullptr) {
    throw std::runtime_error("Hit shape was nullptr");
  }
  Math::Vector3D result(0,0,0);
  std::shared_ptr<AmbientLight> ambientLight;
  std::shared_ptr<DirectionalLight> directionalLight;
  
  for (auto &light: _lights) {
    if (light->getType() == "AmbientLight") {
      ambientLight = std::dynamic_pointer_cast<AmbientLight>(light);
      result = result + light->computeLighting(normal, hitShape, viewDir);
      break;
    }
  }
  
  for (const auto& light : _lights) {
    if (light->getType() == "DirectionalLight") {
      directionalLight = std::dynamic_pointer_cast<DirectionalLight>(light);
      result = result + light->computeLighting(normal, hitShape, viewDir);
    }
  }
  
  if (directionalLight && ambientLight) {
    Math::Vector3D reflectSource = reflect(-directionalLight->direction, normal);
    double specularStrength = std::max(0.0, viewDir.dot(reflectSource));
    specularStrength = std::pow(specularStrength, 32.);
    Math::Vector3D specular = ambientLight->getColor() * specularStrength;
    Math::Vector3D diffuse = ambientLight->getColor() * _diffuse;
    return result * ((diffuse * 0.5 + specular * 0.5) + ambientLight->getIntensity() * 0.0);
  }
  
  return result;
}

void Raytracer::LightComposite::addLight(const std::shared_ptr<ILight> &newLight) {
  _lights.push_back(newLight);
}
