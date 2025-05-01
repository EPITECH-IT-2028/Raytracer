#include "LightComposite.hpp"

Math::Vector3D Raytracer::LightComposite::computeLighting(const Math::Vector3D& normal, const Math::Vector3D& object_color) const {
  Math::Vector3D result(0, 0, 0);
  for (const auto& light : _lights) {
    result = result + light->computeLighting(normal, object_color);
  }
  return result;
}

void Raytracer::LightComposite::addLight(const std::shared_ptr<ILight> &newLight) {
  _lights.push_back(newLight);
}
