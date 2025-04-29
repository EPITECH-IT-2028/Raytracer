#pragma once

#include <memory>
#include <vector>
#include "AShape.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class ShapeComposite : public AShape {
    public:
      ShapeComposite() = default;

      void addShape(const std::shared_ptr<IShape> &shape);

      std::tuple<bool, Math::Vector3D> hits(
          const Raytracer::Ray &ray) const override;

    private:
      std::vector<std::shared_ptr<IShape>> shapes;
  };

}  // namespace Raytracer
