#include "DirectionalLight.hpp"
#include <cmath>
#include "Vector3D.hpp"


Math::Vector3D Raytracer::DirectionalLight::computeLighting(
    const Math::Vector3D& normal, 
    const IShape* hitShape, 
    __attribute__((unused))const Math::Vector3D& viewDir
) const {
  double intensity = std::max(0.0, -normal.dot(direction));
  return hitShape->getColor() * intensity;
}
