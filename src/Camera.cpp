#include "Camera.hpp"
#include <algorithm>
#include <cmath>
#include "Ray.hpp"
#include "Rectangle.hpp"

Raytracer::Ray Raytracer::Camera::ray(double u, double v) {
  Math::Vector3D direction = screen.pointAt(u, v) - origin;
  return Ray(origin, direction);
}

void Raytracer::Camera::updateView() {
  Math::Vector3D world{0, 1, 0};  // Reference vector for the up direction

  _forward = Math::Vector3D{std::cos(_pitch) * std::sin(_yaw), std::sin(_pitch),
                            std::cos(_pitch) * std::cos(_yaw)}
                 .normalize();  // Where the camera is looking
  _right = Math::cross(_forward, world).normalize();
  _up = Math::cross(_right, _forward).normalize();
  _viewportCenter = origin + _forward * _zoom;
  _pixelDeltaU = _right * (_viewportWidth / float(_width));
  _pixelDeltaV = -_up * (_viewportHeight / float(_height));
  _pixel0Location = _viewportCenter - _right * (_viewportWidth * 0.5f) +
                    _up * (_viewportHeight * 0.5f) +
                    (_pixelDeltaU + _pixelDeltaV) * 0.5f;
}
