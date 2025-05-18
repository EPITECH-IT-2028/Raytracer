#pragma once

#include <algorithm>
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Rectangle.hpp"

namespace Raytracer {

  /**
   * @brief Represents a camera in the 3D scene.
   *
   * The Camera class defines the viewpoint from which the scene is rendered.
   * It manages properties like position (origin), orientation (yaw, pitch),
   * field of view, and viewport dimensions. It provides methods to update
   * its view vectors and generate rays for rendering.
   */
  class Camera {
    public:
      Math::Point3D origin; ///< The origin point of the camera.
      Rectangle3D screen;   ///< The screen of the camera.
      float _yaw = -157;    ///< The yaw rotation of the camera.
      float _pitch = -11;   ///< The pitch rotation of the camera.
      Math::Vector3D _right;    ///< The right vector of the camera.
      Math::Vector3D _up;       ///< The up vector of the camera.
      Math::Vector3D _forward;  ///< The forward vector of the camera.

      /**
       * @brief Default constructor for the Camera class.
       */
      Camera() : origin(0, 0, 0), screen(Rectangle3D()) {
      }

      /**
       * @brief Constructor for the Camera class.
       * @param origin The origin point of the camera.
       * @param screen The screen of the camera.
       */
      Camera(const Math::Point3D &origin, const Rectangle3D &screen)
          : origin(origin), screen(screen) {
      }

      /**
       * @brief Destructor for the Camera class.
       */
      ~Camera() = default;

      /**
       * @brief Sets the width of the camera's viewport.
       * @param width The width to set.
       */
      void setWidth(const std::size_t &width) {
        _width = width;
      };

      /**
       * @brief Sets the height of the camera's viewport.
       * @param height The height to set.
       */
      void setHeight(const std::size_t &height) {
        _height = height;
      };

      /**
       * @brief Sets the zoom level of the camera.
       * @param z The zoom level to set.
       */
      void setZoom(const float &z) {
        _zoom = z;
      };

      /**
       * @brief Sets the field of view of the camera.
       * @param fov The field of view to set.
       */
      void setFieldOfView(const float &fov) {
        _fieldOfView = fov;
      };

      /**
       * @brief Sets the viewport height of the camera.
       * @param vh The viewport height to set.
       */
      void setViewportHeight(const float &vh) {
        _viewportHeight = vh;
      };

      /**
       * @brief Sets the viewport width of the camera.
       * @param vw The viewport width to set.
       */
      void setViewportWidth(const float &vw) {
        _viewportWidth = vw;
      };

      /**
       * @brief Gets the width of the camera's viewport.
       * @return The width of the viewport.
       */
      const std::size_t &getWidth() const {
        return _width;
      }

      /**
       * @brief Gets the height of the camera's viewport.
       * @return The height of the viewport.
       */
      const std::size_t &getHeight() const {
        return _height;
      }

      /**
       * @brief Gets the zoom level of the camera.
       * @return The zoom level.
       */
      const float &getZoom() const {
        return _zoom;
      }

      /**
       * @brief Gets the viewport height of the camera.
       * @return The viewport height.
       */
      const float &getViewportHeight() const {
        return _viewportHeight;
      }

      /**
       * @brief Gets the viewport width of the camera.
       * @return The viewport width.
       */
      const float &getViewportWidth() const {
        return _viewportWidth;
      }

      /**
       * @brief Gets the field of view of the camera.
       * @return The field of view.
       */
      const float &getFieldOfView() const {
        return _fieldOfView;
      }

      /**
       * @brief Updates the camera's view based on its current parameters.
       */
      void updateView();

      /**
       * @brief Gets the pixel delta U vector.
       * @return The pixel delta U vector.
       */
      const Math::Vector3D &getPixelDeltaU() const {
        return _pixelDeltaU;
      }

      /**
       * @brief Gets the pixel delta V vector.
       * @return The pixel delta V vector.
       */
      const Math::Vector3D &getPixelDeltaV() const {
        return _pixelDeltaV;
      }

      /**
       * @brief Gets the center of the viewport.
       * @return The viewport center point.
       */
      const Math::Point3D &getViewportCenter() const {
        return _viewportCenter;
      }

      /**
       * @brief Gets the upper-left corner of the viewport.
       * @return The viewport upper-left point.
       */
      const Math::Point3D &getViewportUpperLeft() const {
        return _viewportUpperLeft;
      }

      /**
       * @brief Gets the location of the pixel (0,0).
       * @return The location of the first pixel.
       */
      const Math::Point3D &getPixel0Location() const {
        return _pixel0Location;
      }

      /**
       * @brief Generates a ray for a given pixel.
       * @param u The u coordinate of the pixel.
       * @param v The v coordinate of the pixel.
       * @return The generated ray.
       */
      Ray ray(double u, double v);

      /**
       * @brief Rotates the camera around the Y-axis (yaw).
       * @param a The angle to rotate by.
       */
      void rotateYaw(float a) {
        _yaw += a;
      }

      /**
       * @brief Rotates the camera around the X-axis (pitch).
       * @param a The angle to rotate by.
       */
      void rotatePitch(float a) {
        _pitch += a;
        _pitch = std::clamp(_pitch, -89.9f, 89.9f);
      }

      /**
       * @brief Moves the camera forward.
       * @param a The distance to move.
       */
      void moveForward(float a) {
        origin = origin + _forward * a / 100;
      }

      /**
       * @brief Moves the camera to the right.
       * @param a The distance to move.
       */
      void moveRight(float a) {
        origin = origin + _right * a / 100;
      }

      /**
       * @brief Moves the camera up.
       * @param a The distance to move.
       */
      void moveUp(float a) {
        origin = origin + _up * a / 100;
      }

    private:
      std::size_t _width = 0;             ///< Width of the camera's viewport in pixels.
      std::size_t _height = 0;            ///< Height of the camera's viewport in pixels.
      float _zoom = 2.0;                  ///< Zoom factor (legacy or specific use, consider relation to FoV).
      float _viewportHeight = 2.0;        ///< Height of the viewport in world units.
      float _viewportWidth = 2.0;         ///< Width of the viewport in world units.
      float _fieldOfView = 60.0;          ///< Horizontal field of view in degrees.
      Math::Vector3D _pixelDeltaU;        ///< Change in world space per horizontal pixel.
      Math::Vector3D _pixelDeltaV;        ///< Change in world space per vertical pixel.
      Math::Point3D _viewportCenter;      ///< Center point of the viewport in world space.
      Math::Point3D _viewportUpperLeft;   ///< Upper-left corner of the viewport in world space.
      Math::Point3D _pixel0Location;      ///< Location of the center of the top-left pixel (pixel 0,0) in world space.
  };

}  // namespace Raytracer
