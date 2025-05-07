#include "LightComposite.hpp"
#include <cmath>
#include <memory>
#include "Vector3D.hpp"

const Math::Vector3D Raytracer::LightComposite::reflect(const Math::Vector3D &incident, const Math::Vector3D &normal) const {
  Math::Vector3D n = normal.normalized();
  return incident - (n * (incident.dot(normal) * 2));
}

Math::Vector3D Raytracer::LightComposite::computeLighting(
  const Math::Vector3D& normal,
  const Math::Vector3D &objectColor,
  const Math::Point3D &hitPoint,
  const Math::Vector3D &viewDir,
  const ShapeComposite &shapes 
) {
  Math::Vector3D result(0,0,0);
  std::shared_ptr<ILight> ambientLight;
  std::shared_ptr<ILight> directionalLight;
  
  for (auto &light: _lights) {
    if (light->getType() == "AmbientLight") {
      ambientLight = light;
      result = result + light->computeLighting(normal, objectColor, hitPoint, viewDir, shapes);
      break;
    }
  }
  
  for (const auto& light : _lights) {
    if (light->getType() == "DirectionalLight") {
      directionalLight = light;
      result = result + light->computeLighting(normal, objectColor, hitPoint, viewDir, shapes);
    }
  }
  
  if (directionalLight.get() && ambientLight.get()) {
    Math::Vector3D reflectSource = reflect(-directionalLight->getDirection(), normal);
    double specularStrength = std::max(0.0, viewDir.dot(reflectSource));
    specularStrength = std::pow(specularStrength, 32.);
    Math::Vector3D specular = ambientLight->getColor() * specularStrength;
    Math::Vector3D diffuse = ambientLight->getColor() * _diffuse;
    return result * ((diffuse * 0.5 + specular * 0.5) * ambientLight->getIntensity());
  }
  return result;
}

void Raytracer::LightComposite::addLight(
    const std::shared_ptr<ILight>& newLight) {
  _lights.push_back(newLight);
}
