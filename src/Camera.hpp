#pragma once

#include <iostream>
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Rectangle.hpp"

namespace Raytracer {

  class Camera {
    public:
      Math::Point3D origin;
      Rectangle3D screen;
      float _yaw = -157;
      float _pitch = -11;
      Math::Vector3D _right;
      Math::Vector3D _up;
      Math::Vector3D _forward;

      Camera() : origin(0, 0, 0), screen(Rectangle3D()) {
      }

      Camera(const Math::Point3D &origin, const Rectangle3D &screen)
          : origin(origin), screen(screen) {
      }

      ~Camera() = default;

      void setWidth(const std::size_t &width) {
        _width = width;
      };

      void setHeight(const std::size_t &height) {
        _height = height;
      };

      void setZoom(const float &z) {
        _zoom = z;
      };

      void setFieldOfView(const float &fov) {
        _fieldOfView = fov;
      };

      void setViewportHeight(const float &vh) {
        _viewportHeight = vh;
      };

      void setViewportWidth(const float &vw) {
        _viewportWidth = vw;
      };

      const std::size_t &getWidth() const {
        return _width;
      }

      const std::size_t &getHeight() const {
        return _height;
      }

      const float &getZoom() const {
        return _zoom;
      }

      const float &getViewportHeight() const {
        return _viewportHeight;
      }

      const float &getViewportWidth() const {
        return _viewportWidth;
      }

      const float &getFieldOfView() const {
        return _fieldOfView;
      }

      void updateView();

      const Math::Vector3D &getPixelDeltaU() const {
        return _pixelDeltaU;
      }

      const Math::Vector3D &getPixelDeltaV() const {
        return _pixelDeltaV;
      }

      const Math::Point3D &getViewportCenter() const {
        return _viewportCenter;
      }

      const Math::Point3D &getViewportUpperLeft() const {
        return _viewportUpperLeft;
      }

      const Math::Point3D &getPixel0Location() const {
        return _pixel0Location;
      }

      Ray ray(double u, double v);

      void rotateYaw(float a) {
        _yaw += a;
      }

      void rotatePitch(float a) {
        _pitch += a;
        _pitch = std::max(-89.9f, std::min(89.9f, _pitch));
      }

      void moveForward(float delta) {
        origin = origin + _forward * delta;
      }

      void moveRight(float delta) {
        origin = origin + _right * delta;
      }

      void moveUp(float delta) {
        origin = origin + _up * delta;
      }

    private:
      std::size_t _width = 0;
      std::size_t _height = 0;
      float _zoom = 2.0;
      float _viewportHeight = 2.0;
      float _viewportWidth = 2.0;
      float _fieldOfView = 0;
      Math::Vector3D _pixelDeltaU;
      Math::Vector3D _pixelDeltaV;
      Math::Point3D _viewportCenter;
      Math::Point3D _viewportUpperLeft;
      Math::Point3D _pixel0Location;
  };

}  // namespace Raytracer
