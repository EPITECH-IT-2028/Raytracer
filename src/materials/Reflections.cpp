#include "Reflections.hpp"
#include "Vector3D.hpp"

Math::Vector3D Raytracer::Reflections::reflect(
    const Math::Vector3D &incident, const Math::Vector3D &normal) const {
  return incident - normal * (2 * incident.dot(normal));
}

Math::Vector3D Raytracer::Reflections::computeMaterial(
    const Math::Vector3D &normal, const Math::Vector3D &viewDir,
    const Math::Point3D &hitPoint, const Math::Vector3D &color) const {
  (void)hitPoint;
  float coef = 1;

  Math::Vector3D reflected = reflect(-viewDir, normal).normalize();
  double skyFactor = 0.5 * (reflected.y + 1.0);
  Math::Vector3D environmentColor =
      Math::Vector3D(1.0, 1.0, 1.0) * (1.0 - skyFactor) +
      Math::Vector3D(0.5, 0.7, 1.0) * skyFactor;
  return color * (1.0 - coef) + environmentColor * coef;
}

extern "C" {
Raytracer::IMaterials *addMaterial() {
  return new Raytracer::Reflections();
}
}
