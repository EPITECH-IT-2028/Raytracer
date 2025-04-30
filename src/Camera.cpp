#include "Camera.hpp"
#include "Ray.hpp"

Raytracer::Ray Raytracer::Camera::ray(double u, double v) {
  Math::Vector3D direction = screen.pointAt(u, v) - origin;
  return Ray(origin, direction);
}

void Raytracer::Camera::updateView() {
  _viewportU = Math::Vector3D(_viewportWidth, 0, 0);
  _viewportV = Math::Vector3D(0, -_viewportHeight, 0);

  _pixelDeltaU = _viewportU / static_cast<float>(_width);
  _pixelDeltaV = _viewportV / static_cast<float>(_height);
  _viewportCenter = origin + Math::Vector3D(0, 0, -_zoom);
  _viewportUpperLeft = _viewportCenter - _viewportU / 2.0f - _viewportV / 2.0f;
  _pixel0Location =
      _viewportUpperLeft + _pixelDeltaU / 2.0f + _pixelDeltaV / 2.0f;
}
