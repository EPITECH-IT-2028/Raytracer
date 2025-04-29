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

      void updateView();

      const Math::Vector3D &getViewportU() const {
        return _viewportU;
      }

      const Math::Vector3D &getViewportV() const {
        return _viewportV;
      }

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

      const Math::Point3D &getPixel00Location() const {
        return _pixel0Location;
      }

      Ray ray(double u, double v);

    private:
      std::size_t _width;
      std::size_t _height;
      float _zoom;
      float _viewportHeight;
      float _viewportWidth;
      Math::Vector3D _viewportU;
      Math::Vector3D _viewportV;
      Math::Vector3D _pixelDeltaU;
      Math::Vector3D _pixelDeltaV;
      Math::Point3D _viewportCenter;
      Math::Point3D _viewportUpperLeft;
      Math::Point3D _pixel0Location;
  };

}  // namespace Raytracer
