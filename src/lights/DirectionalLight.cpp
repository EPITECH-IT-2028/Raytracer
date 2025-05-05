#include "DirectionalLight.hpp"
#include "Vector3D.hpp"

Math::Vector3D Raytracer::DirectionalLight::computeLighting(
    const Math::Vector3D& normal, const Math::Vector3D& objectColor,
    const Math::Point3D& hitPoint, const ShapeComposite& shapes) const {
  Math::Point3D shadowOrigin = hitPoint + normal * 0.001;
  Raytracer::Ray shadowRay(shadowOrigin, -direction);
  auto [shadow, _, shadowShape] = shapes.hits(shadowRay);

  if (shadow > 0.0 && shadowShape != nullptr)
    return objectColor * 0.1;

  return objectColor * std::max(0.0, normal.dot(-direction));
}
