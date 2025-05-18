#pragma once

#include "ILight.hpp"
#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

namespace Raytracer {
  /**
   * @brief Abstract base class for all light sources in the Raytracer.
   *
   * This class implements the ILight interface and provides common
   * properties and methods for lights, such as intensity, color,
   * direction (for directional lights), and type.
   */
  class ALight : public ILight {
    public:
      /**
       * @brief Virtual default destructor for ALight.
       */
      virtual ~ALight() = default;

      /**
       * @brief Pure virtual method to compute the lighting contribution of this light.
       * @param normal The surface normal at the hit point.
       * @param objectColor The color of the object at the hit point.
       * @param hitPoint The point on the surface where the light is being calculated.
       * @param viewDir The direction from the hit point to the camera.
       * @param shapes A composite of all shapes in the scene, used for shadow checking.
       * @return Math::Vector3D The calculated color contribution from this light.
       */
      virtual Math::Vector3D computeLighting(
          const Math::Vector3D &normal, const Math::Vector3D &objectColor,
          const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
          const ShapeComposite &shapes) const override = 0;

      /**
       * @brief Sets the intensity of the light.
       * @param intensity The new intensity value.
       */
      void setIntensity(double intensity) {
        _intensity = intensity;
      };
      /**
       * @brief Sets the color of the light.
       * @param color The new color vector.
       */
      void setColor(const Math::Vector3D &color) {
        _color = color;
      };
      /**
       * @brief Sets the direction of the light (relevant for directional lights).
       * @param direction The new direction vector.
       */
      void setDirection(const Math::Vector3D &direction) {
        _direction = direction;
      };
      /**
       * @brief Sets the type identifier string for the light.
       * @param type The string representing the light type (e.g., "PointLight").
       */
      void setType(const std::string &type) {
        _type = type;
      }

      /**
       * @brief Gets the type identifier string of the light.
       * @return const std::string& A const reference to the light's type string.
       */
      const std::string &getType() const override {
        return _type;
      };
      /**
       * @brief Gets the direction of the light.
       * @return Math::Vector3D The light's direction vector.
       */
      Math::Vector3D getDirection() const override {
        return _direction;
      };
      /**
       * @brief Gets the color of the light.
       * @return const Math::Vector3D& A const reference to the light's color.
       */
      const Math::Vector3D &getColor() const override {
        return _color;
      };
      /**
       * @brief Gets the intensity of the light.
       * @return double The light's intensity value.
       */
      double getIntensity() const override {
        return _intensity;
      };

    protected:
      std::string _type;          ///< String identifier for the type of light.
      Math::Vector3D _direction;  ///< Direction vector for the light (primarily for directional lights).
      Math::Vector3D _color;      ///< Color of the light.
      double _intensity = 0;      ///< Intensity of the light.
  };
}  // namespace Raytracer
