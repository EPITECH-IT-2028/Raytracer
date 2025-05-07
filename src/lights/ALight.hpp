#pragma once

#include "ILight.hpp"
#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class ALight : public ILight {
    public:
      virtual ~ALight() = default;

      virtual Math::Vector3D computeLighting(
          const Math::Vector3D &normal, const Math::Vector3D &objectColor,
          const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
          const ShapeComposite &shapes) override = 0;
      void setIntensity(double intensity) {
        _intensity = intensity;
      };
      void setColor(const Math::Vector3D &color) {
        _color = color;
      };
      void setDirection(const Math::Vector3D &direction) {
        _direction = direction;
      };
      void setType(const std::string &type) {
        _type = type;
      }

      const std::string &getType() const override {
        return _type;
      };
      Math::Vector3D getDirection() const override {
        return _direction;
      };
      const Math::Vector3D &getColor() const override {
        return _color;
      };
      double getIntensity() const override {
        return _intensity;
      };

    protected:
      std::string _type;
      Math::Vector3D _direction;
      Math::Vector3D _color;
      double _intensity;
  };
}  // namespace Raytracer
