#pragma once
#include <memory>
#include <vector>
#include "ALight.hpp"
#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class LightComposite : public ALight {
    public:
      LightComposite() = default;
      ~LightComposite() = default;

      void addLight(const std::shared_ptr<ILight> &);

      Math::Vector3D computeLighting(const Math::Vector3D &normal,
                                     const Math::Vector3D &objectColor,
                                     const Math::Point3D &hitPoint,
                                     const Math::Vector3D &viewDir,
                                     const ShapeComposite &shapes) override;

      const std::vector<std::shared_ptr<ILight>> &getLights() const {
        return _lights;
      };

      void setDiffuse(double diffuse) {
        _diffuse = diffuse;
      }
      double getDiffuse() const {
        return _diffuse;
      }
      const Math::Vector3D reflect(const Math::Vector3D &light,
                                   const Math::Vector3D &normal) const;

    private:
      std::vector<std::shared_ptr<ILight>> _lights;
      double _diffuse = 0;
  };
};  // namespace Raytracer
