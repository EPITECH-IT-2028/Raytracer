#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

void Raytracer::ShapeComposite::addShape(const std::shared_ptr<IShape> &shape) {
  shapes.push_back(shape);
}

std::tuple<bool, Math::Vector3D> Raytracer::ShapeComposite::hits(
    const Raytracer::Ray &ray) const {
  for (const auto &shape : shapes) {
    auto [hit, color] = shape->hits(ray);
    if (hit) {
      return {true, color};
    }
  }
  return {false, Math::Vector3D(0, 0, 0)};
}
