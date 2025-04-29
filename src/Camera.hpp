#pragma once

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

      Ray ray(double u, double v);
  };

}  // namespace Raytracer
