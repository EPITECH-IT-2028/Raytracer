#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

void Raytracer::ShapeComposite::addShape(const std::shared_ptr<IShape> &shape) {
  shapes.push_back(shape);
}

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::ShapeComposite::hits(const Raytracer::Ray &ray) const {
  double closestT = 100;  // TODO: Use a more appropriate value
  Math::Vector3D hitColor;
  const IShape *hitShape = nullptr;

  for (const auto &shape : shapes) {
    auto [t, color, s] = shape->hits(ray);
    if (t > 0.0 && t < closestT) {
      closestT = t;
      hitColor = color;
      hitShape = s;
    }
  }
  if (hitShape) {
    return {closestT, hitColor, hitShape};
  }
  return {0.0, Math::Vector3D(0, 0, 0), nullptr};
}
