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

      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return Math::Vector3D(0, 0, 0);
      }

     const std::vector<std::shared_ptr<IShape>>& getShapes() const { return shapes; };

     void translate(const Math::Vector3D &offset) override {
        for (const auto &shape : shapes)
          shape->translate(offset);
     }

    private:
      std::vector<std::shared_ptr<IShape>> shapes;
  };

}  // namespace Raytracer
