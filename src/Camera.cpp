#include "Camera.hpp"
#include <cmath>
#include "Ray.hpp"
#include "Rectangle.hpp"

/**
 * @brief Generates a ray from the camera origin through a point (u,v) on the screen plane.
 * @param u The horizontal parametric coordinate on the screen plane (typically [0,1]).
 * @param v The vertical parametric coordinate on the screen plane (typically [0,1]).
 * @return Raytracer::Ray The generated ray.
 */
Raytracer::Ray Raytracer::Camera::ray(double u, double v) {
  Math::Vector3D direction = screen.pointAt(u, v) - origin;
  return Ray(origin, direction);
}

/**
 * @brief Updates the camera's view parameters based on its current orientation and position.
 *
 * This method recalculates the forward, right, and up vectors, the viewport center,
 * and pixel delta vectors. This should be called whenever the camera's position,
 * orientation (pitch, yaw), or zoom changes.
 */
void Raytracer::Camera::updateView() {
  const float DEG_TO_RAD = M_PI / 180.0f;
  float pitch_rad = _pitch * DEG_TO_RAD;
  float yaw_rad = _yaw * DEG_TO_RAD;
  Math::Vector3D world{0, 1, 0};  // Reference vector for the up direction

  _forward = Math::Vector3D{std::cos(pitch_rad) * std::sin(yaw_rad),
                            std::sin(pitch_rad),
                            std::cos(pitch_rad) * std::cos(yaw_rad)}
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
