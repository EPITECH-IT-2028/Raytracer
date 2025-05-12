#pragma once
#include "ALight.hpp"

namespace Raytracer {
class PointLight : public ALight {
  public:
    PointLight() = default;
    ~PointLight() = default;

    Math::Vector3D computeLighting(
      const Math::Vector3D &normal, const Math::Vector3D &objectColor,
      const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
      const ShapeComposite &shapes) const override;

    void setPosition(const Math::Point3D &position) {
        _position = position;
    }
  private:
    Math::Point3D _position;
};
}