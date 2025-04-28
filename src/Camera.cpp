#include "Camera.hpp"
#include "Ray.hpp"

Raytracer::Ray Raytracer::Camera::ray(double u, double v) {
  Math::Vector3D direction = screen.pointAt(u, v) - origin;
  return Ray(origin, direction);
}
