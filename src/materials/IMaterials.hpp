#pragma once

#include <iostream>
#include "Camera.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  class Renderer;
  class ShapeComposite;
  class LightComposite;
  class Camera;

  class IMaterials {
    public:
      virtual ~IMaterials() = default;

      using RayColorFunc = std::function<Math::Vector3D(
          Raytracer::Ray &, const Raytracer::ShapeComposite &,
          Raytracer::LightComposite &, const Raytracer::Camera &, int)>;

      virtual Math::Vector3D computeMaterial(
          const Math::Vector3D &normal, const Math::Vector3D &viewDir,
          const Math::Point3D &hitPoint, const Math::Vector3D &color,
          const Raytracer::ShapeComposite &shapes,
          Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
          int depth, RayColorFunc rayColorFunc) const = 0;
  };
}  // namespace Raytracer
