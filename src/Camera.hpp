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

      Ray ray(double u, double v);

    private:
      std::size_t _width;
      std::size_t _height;
      float _zoom;
      float _viewportHeight;
      float _viewportWidth;
  };

}  // namespace Raytracer
