#pragma once
#include <memory>
#include <vector>
#include "ILight.hpp"

namespace Raytracer {
  class LightComposite : public ILight {
    public:
      LightComposite() = default;
      ~LightComposite() = default;

      void addLight(const std::shared_ptr<ILight>&);

      Math::Vector3D computeLighting(const Math::Vector3D& normal,
                                     const Math::Vector3D& objectColor,
                                     const Math::Point3D& hitPoint,
                                     const ShapeComposite& shapes) override;

      const std::vector<std::shared_ptr<ILight>>& getLights() const {
        return _lights;
      };

    private:
      std::vector<std::shared_ptr<ILight>> _lights;
  };
};  // namespace Raytracer
