#pragma once

#include "ILight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class AmbientLight : public ILight {
    public:
      AmbientLight() = default;
      ~AmbientLight() = default;

      double getIntensity() const { return _intensity; };
      const Math::Vector3D &getColor() const { return _color;};
      const std::string &getType() const override { return _type; } ;

      void setIntensity(double intensity) { _intensity = intensity; };
      void setColor(const Math::Vector3D &color) { _color = color; };

      Math::Vector3D computeLighting(const Math::Vector3D& normal, const IShape *hitShape, const Math::Vector3D &viewDir) const override;

    private:
      double _intensity = 1;
      Math::Vector3D _color = Math::Vector3D(0.5,0.5,0.5);
      const std::string _type = "AmbientLight";
  };
}  // namespace Raytracer
