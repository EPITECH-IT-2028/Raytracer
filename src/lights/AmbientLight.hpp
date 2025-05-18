#pragma once

#include "ALight.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  /**
   * @brief Represents an ambient light source in the scene.
   *
   * This class inherits from ALight and provides a constant light
   * contribution to all objects in the scene, regardless of their position,
   * orientation, or shadows. It simulates indirect lighting.
   */
  class AmbientLight : public ALight {
    public:
      /**
       * @brief Default constructor for AmbientLight.
       */
      AmbientLight() = default;
      /**
       * @brief Default destructor for AmbientLight.
       */
      ~AmbientLight() = default;

      /**
       * @brief Computes the lighting contribution of this ambient light.
       *
       * For ambient light, this typically involves multiplying the object's color
       * by the light's color and intensity.
       * @param normal The surface normal (unused for ambient light).
       * @param objectColor The color of the object at the hit point.
       * @param hitPoint The point on the surface (unused for ambient light).
       * @param viewDir The direction from the hit point to the camera (unused for ambient light).
       * @param shapes A composite of all shapes in the scene (unused for ambient light).
       * @return Math::Vector3D The calculated color contribution from this ambient light.
       */
      Math::Vector3D computeLighting(
          const Math::Vector3D &normal, const Math::Vector3D &objectColor,
          const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
          const ShapeComposite &shapes) const override;
  };
}  // namespace Raytracer
