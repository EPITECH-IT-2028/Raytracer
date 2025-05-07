#include "Cone.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include <cmath>

// std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
// Raytracer::Cone::hits(const Raytracer::Ray &ray) const {
//     double d_dot_v = ray.direction.dot(_normal);
//     double x_dot_v = (ray.origin - _center).dot(_normal);

//     Math::Vector3D d_perp = ray.direction - _normal * d_dot_v;
//     Math::Vector3D x_perp = (ray.origin - _center) - _normal * x_dot_v;

//     double k = _radius / _height;
//     double k2 = k * k;

//     double a = d_perp.dot(d_perp) - k2 * d_dot_v * d_dot_v;
//     double b = 2.0 * (d_perp.dot(x_perp) - k2 * d_dot_v * x_dot_v);
//     double c = x_perp.dot(x_perp) - k2 * x_dot_v * x_dot_v;

//     double discriminant = b * b - 4 * a * c;
//     if (discriminant < 0.0) {
//         return {0.0, _color, this};
//     }
//     double sqrt_disc = sqrt(discriminant);
//     double t1 = (-b - sqrt_disc) / (2.0 * a);
//     double t2 = (-b + sqrt_disc) / (2.0 * a);

//     if (t1 > t2) std::swap(t1, t2);

//     double t_plane_A = -x_dot_v / d_dot_v;
//     double t_plane_B = (_height - x_dot_v) / d_dot_v;

//     double t3 = std::min(t_plane_A, t_plane_B);
//     double t4 = std::max(t_plane_A, t_plane_B);

//     double final_t_start = std::max(t1, t3);
//     double final_t_end = std::min(t2, t4);

//     if (final_t_start < final_t_end && final_t_end >= 0) {
//         return {final_t_start, _color, this};
//     }
//     return {0.0, _color, this};
// }

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Cone::hits(const Raytracer::Ray &ray) const {
    double height_ = _height;
    Math::Vector3D normal_ = _normal;

    if (_height <= 0.0) {
      height_ *= -1.0;
      normal_ = -_normal;
    }

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
    double best_t = -1.0;
    Math::Vector3D normal;

    if (discriminant >= 0.0) {
      double sqrt_disc = sqrt(discriminant);
      double t1 = (-b - sqrt_disc) / (2.0 * a);
      double t2 = (-b + sqrt_disc) / (2.0 * a);
      if (t1 > t2) std::swap(t1, t2);

      for (double t : {t1, t2}) {
          if (t < 0.0) continue;
          Math::Point3D p = ray.origin + ray.direction * t;
          double m = (p - _center).dot(_normal);
          if (m >= 0.0 && m <= _height) {
              best_t = t;
              normal = (p - _center) - _normal * (m * (1.0 + k2));
              normal = normal.normalize();
              break;
          }
      }
  }

    // === Cap sommet (m = 0) ===
    if (d_dot_v != 0.0) {
        double t_top = -x_dot_v / d_dot_v;
        if (t_top >= 0.0) {
            Math::Point3D p = ray.origin + ray.direction * t_top;
            // On considère que si on frappe le plan au bon endroit, on touche le sommet
            double dist_to_center = (p - _center).length();
            if (dist_to_center <= 1e-6) {
                if (best_t < 0 || t_top < best_t) {
                    best_t = t_top;
                    normal = -_normal; // direction opposée à l'axe
                }
            }
        }

        // === Cap base (m = _height) ===
        double t_base = (_height - x_dot_v) / d_dot_v;
        if (t_base >= 0.0) {
            Math::Point3D p = ray.origin + ray.direction * t_base;
            Math::Vector3D v = p - _center;
            double m = v.dot(_normal);
            Math::Vector3D v_perp = v - _normal * m;

            double r_at_m = m * k;
            if (v_perp.dot(v_perp) <= r_at_m * r_at_m) {
                if (best_t < 0 || t_base < best_t) {
                    best_t = t_base;
                    normal = _normal; // normal sortante
                }
            }
        }
    }

    if (best_t >= 0.0) {
        return {best_t, normal, this};
    }

    return {0.0, Math::Vector3D(), this};
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