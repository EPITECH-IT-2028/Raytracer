#pragma once
#include "ALight.hpp"

namespace Raytracer {
/**
 * @brief Represents a point light source in the scene.
 *
 * This class inherits from ALight and provides specific implementation
 * for a light source that emanates light in all directions from a single point in space.
 * It contributes to diffuse and specular lighting and can cast shadows.
 */
class PointLight : public ALight {
  public:
    /**
     * @brief Default constructor for PointLight.
     */
    PointLight() = default;
    /**
     * @brief Default destructor for PointLight.
     */
    ~PointLight() = default;

    /**
     * @brief Computes the lighting contribution of this point light at a given hit point.
     *
     * @param normal The surface normal at the hit point.
     * @param objectColor The color of the object at the hit point.
     * @param hitPoint The point on the surface where the light is being calculated.
     * @param viewDir The direction from the hit point to the camera.
     * @param shapes A composite of all shapes in the scene, used for shadow checking.
     * @return Math::Vector3D The calculated color contribution from this light.
     */
    Math::Vector3D computeLighting(
      const Math::Vector3D &normal, const Math::Vector3D &objectColor,
      const Math::Point3D &hitPoint, const Math::Vector3D &viewDir,
      const ShapeComposite &shapes) const override;

    /**
     * @brief Sets the position of the point light.
     *
     * @param position The new position for the light.
     */
    void setPosition(const Math::Point3D &position) {
        _position = position;
    }
    /**
     * @brief Gets the position of the point light.
     *
     * @return Math::Point3D& A reference to the light's position.
     */
    Math::Point3D &getPosition() { return _position;};
  private:
    Math::Point3D _position; ///< The 3D position of the point light.
};
}
