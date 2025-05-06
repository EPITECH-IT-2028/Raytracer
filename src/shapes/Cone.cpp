#include "Cone.hpp"
#include "Vector3D.hpp"
#include <cmath>

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Cone::hits(const Raytracer::Ray &ray) const {
    double d_dot_v = ray.direction.dot(_normal);
    double x_dot_v = (ray.origin - _center).dot(_normal);

    Math::Vector3D d_perp = ray.direction - _normal * d_dot_v;
    Math::Vector3D x_perp = (ray.origin - _center) - _normal * x_dot_v;

    double k = _radius / _height;
    double k2 = k * k;

    double a = d_perp.dot(d_perp) - k2 * d_dot_v * d_dot_v;
    double b = 2.0 * (d_perp.dot(x_perp) - k2 * d_dot_v * x_dot_v);
    double c = x_perp.dot(x_perp) - k2 * x_dot_v * x_dot_v;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0) {
        return {0.0, _color, this};
    }
    double sqrt_disc = sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2.0 * a);
    double t2 = (-b + sqrt_disc) / (2.0 * a);

    if (t1 > t2) std::swap(t1, t2);

    double t_plane_A = -x_dot_v / d_dot_v;
    double t_plane_B = (_height - x_dot_v) / d_dot_v;

    double t3 = std::min(t_plane_A, t_plane_B);
    double t4 = std::max(t_plane_A, t_plane_B);

    double final_t_start = std::max(t1, t3);
    double final_t_end = std::min(t2, t4);

    if (final_t_start < final_t_end && final_t_end >= 0) {
        return {final_t_start, _color, this};
    }
    return {0.0, _color, this};
}

extern "C" {
  Raytracer::IShape *addShape() {
    try {
      return new Raytracer::Cone();
    } catch (const std::exception &e) {
      std::cerr << "[ERROR] - Failed to create cone: " << e.what()
                << std::endl;
      return nullptr;
    }
  }
}