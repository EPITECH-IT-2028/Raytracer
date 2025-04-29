#pragma once

#include "Ray.hpp"

namespace Raytracer {

class IShape {
public:
  virtual ~IShape() = default;

  virtual bool hits(const Ray &ray) const = 0;
};

} // namespace Raytracer
