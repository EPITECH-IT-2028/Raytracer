#pragma once
#include <memory>
#include <vector>
#include "ILight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
class LightComposite : public ILight {
public:
  LightComposite() = default; 

  void addLight(const std::shared_ptr<ILight> &);
  Math::Vector3D computeLighting(const Math::Vector3D &normal, const IShape *hitShape, const Math::Vector3D &viewDir) const override;
  const std::string &getType() const override { return _type; }
  const std::vector<std::shared_ptr<ILight>>& getLights() const { return _lights;};
  void setDiffuse(double diffuse) { _diffuse = diffuse;}
  double getDiffuse() const {return _diffuse;}
  const Math::Vector3D reflect(const Math::Vector3D &light, const Math::Vector3D &normal) const;
private:
  std::vector<std::shared_ptr<ILight>> _lights;
  const std::string _type = "LightComposite";
  double _diffuse = 0;
};
};
