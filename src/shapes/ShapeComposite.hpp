#pragma once

#include "AShape.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class ShapeGroup : public AShape {
public:
  ShapeGroup() = default;

  void addShape(const std::shared_ptr<IShape> &shape);

  bool hits(const Raytracer::Ray &ray) const override;

private:
  std::vector<std::shared_ptr<IShape>> shapes;
};

} // namespace Raytracer
