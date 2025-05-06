#pragma once

#include "ILight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class DirectionalLight : public ILight {
    public:
      Math::Vector3D direction;

      DirectionalLight() = default;
      DirectionalLight(Math::Vector3D direction) : direction(direction) {};
      ~DirectionalLight() = default;
      const Math::Vector3D reflect(const Math::Vector3D &light, const Math::Vector3D &normal) const;
      Math::Vector3D computeLighting(const Math::Vector3D& normal, const IShape *hitShape, const Math::Vector3D &viewDir) const override;
      const std::string &getType() const override { return _type; } ;
    private:
      const std::string _type = "DirectionalLight";
  };
}  // namespace Raytracer
