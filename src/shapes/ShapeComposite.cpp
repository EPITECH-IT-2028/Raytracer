#include "ShapeComposite.hpp"

void Raytracer::ShapeGroup::addShape(const std::shared_ptr<IShape> &shape) {
  shapes.push_back(shape);
}

bool Raytracer::ShapeGroup::hits(const Raytracer::Ray &ray) const {
  for (const auto &shape : shapes) {
    if (shape->hits(ray)) {
      return true;
    }
  }
  return false;
}
