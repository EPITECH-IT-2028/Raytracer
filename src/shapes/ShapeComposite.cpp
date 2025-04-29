#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

void Raytracer::ShapeComposite::addShape(const std::shared_ptr<IShape> &shape) {
  shapes.push_back(shape);
}

std::tuple<double, Math::Vector3D> Raytracer::ShapeComposite::hits(
    const Raytracer::Ray &ray) const {
  for (const auto &shape : shapes) {
    auto [t, color] = shape->hits(ray);
    if (t > 0.0) {
      return {t, color};
    }
  }
  return {0.0, Math::Vector3D(0, 0, 0)};
}
