#pragma once

#include "IShape.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

class AShape : public IShape {
public:
  Math::Vector3D color;

  virtual ~AShape() = default;

  virtual bool hits(const Ray &ray) const = 0;
};

} // namespace Raytracer
