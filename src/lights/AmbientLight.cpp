#include "AmbientLight.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"

Math::Vector3D Raytracer::AmbientLight::computeLighting(
    const Math::Vector3D &normal, const Math::Vector3D &objectColor,
    const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
    const ShapeComposite &shapes) {
  (void)normal;
  (void)viewDir;
  (void)hitPoint;
  (void)shapes;

  return objectColor * (_color * _intensity);
}

extern "C" {
Raytracer::ILight *addLight() {
  try {
    return new Raytracer::AmbientLight();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create AmbientLight: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
