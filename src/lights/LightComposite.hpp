#pragma once
#include <memory>
#include <vector>
#include "ILight.hpp"

namespace Raytracer {
class LightComposite : public ILight {
public:
  LightComposite() = default; 
  ~LightComposite() = default;
  void addLight(const std::shared_ptr<ILight> &);
  Math::Vector3D computeLighting(const Math::Vector3D& normal, const Math::Vector3D& object_color) const;
  const std::vector<std::shared_ptr<ILight>>& getLights() const { return _lights;};
private:
  std::vector<std::shared_ptr<ILight>> _lights;
};
};
