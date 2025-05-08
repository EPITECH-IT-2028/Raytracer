#include "Plane.hpp"

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Plane::hits(const Raytracer::Ray &ray) const {
  float denominator = _normal.dot(ray.direction);

  if (std::abs(denominator) > 1e-6) {
    Math::Vector3D vectorToPlane = _center - ray.origin;
    float t = vectorToPlane.dot(_normal) / denominator;
    return {t, _color, this};
  }
  return {0.0, _color, nullptr};
}

extern "C" {
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Plane();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Plane object: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
