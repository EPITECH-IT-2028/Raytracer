#include "Cone.hpp"
#include <cmath>
#include <iostream>
#include "Point3D.hpp"
#include "Vector3D.hpp"

const double eps = 1e-6;

std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::Cone::hits(const Raytracer::Ray &ray) const {
  double cone_height = _height;
  Math::Vector3D cone_axis = _normal;

  if (_height < 0) {
    cone_height = -_height;
    cone_axis = -_normal;
  }

  Math::Vector3D oc = ray.origin - _center;
  double oc_dot_normal = oc.dot(cone_axis);
  Math::Vector3D oc_perp = oc - cone_axis * oc_dot_normal;

  double dir_dot_normal = ray.direction.dot(cone_axis);
  Math::Vector3D dir_perp = ray.direction - cone_axis * dir_dot_normal;

  double tan_angle = _radius / cone_height;
  double tan_angle2 = tan_angle * tan_angle;

  double a =
      dir_perp.dot(dir_perp) - tan_angle2 * dir_dot_normal * dir_dot_normal;
  double b = 2.0 * (dir_perp.dot(oc_perp) - tan_angle2 * dir_dot_normal *
                                                (oc_dot_normal - cone_height));
  double c = oc_perp.dot(oc_perp) - tan_angle2 * (oc_dot_normal - cone_height) *
                                        (oc_dot_normal - cone_height);

  double t_side = 0.0;
  double discriminant = b * b - 4 * a * c;

  if (discriminant >= 0.0) {
    double sqrt_disc = sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2.0 * a);
    double t2 = (-b + sqrt_disc) / (2.0 * a);

    if (t1 > 0 && t2 > 0) {
      t_side = std::min(t1, t2);
    } else if (t1 > 0) {
      t_side = t1;
    } else if (t2 > 0) {
      t_side = t2;
    }

    if (t_side > 0) {
      Math::Point3D hit_point = ray.origin + ray.direction * t_side;
      Math::Vector3D cp = hit_point - _center;
      double height_pos = cp.dot(cone_axis);

      if (height_pos < 0 || height_pos > cone_height) {
        t_side = 0.0;
      }
    }
  }

  double t_base = 0.0;
  if (std::abs(dir_dot_normal) > eps) {
    double t_plane = -oc_dot_normal / dir_dot_normal;

    if (t_plane > 0) {
      Math::Point3D hit_point = ray.origin + ray.direction * t_plane;
      Math::Vector3D cp = hit_point - _center;
      Math::Vector3D cp_perp = cp - cone_axis * cp.dot(cone_axis);

      if (cp_perp.length() <= _radius) {
        t_base = t_plane;
      }
    }
  }

  double final_t = 0.0;

  if (t_side > 0 && t_base > 0) {
    final_t = std::min(t_side, t_base);
  } else if (t_side > 0) {
    final_t = t_side;
  } else if (t_base > 0) {
    final_t = t_base;
  }

  if (final_t <= 0) {
    return {0.0, _color, this};
  }

  return {final_t, _color, this};
}

Math::Vector3D Raytracer::Cone::getNormal(
    const Math::Point3D &hit_point) const {
  double cone_height = _height;
  Math::Vector3D cone_axis = _normal;
  if (_height < 0) {
    cone_height = -_height;
    cone_axis = -_normal;
  }

  Math::Vector3D cp = hit_point - _center;

  double height_pos = cp.dot(cone_axis);

  if (std::abs(height_pos) < eps)
    return -cone_axis;

  Math::Point3D pos_on_axis = _center + cone_axis * height_pos;
  Math::Vector3D radial_dir = hit_point - pos_on_axis;

  if (radial_dir.length() < eps) {
    return cone_axis;
  }

  radial_dir = radial_dir.normalize();

  double tan_angle = _radius / cone_height;
  double tan_angle2 = tan_angle * tan_angle;
  double sin_angle = tan_angle / std::sqrt(1 + tan_angle2);
  double cos_angle = 1 / std::sqrt(1 + tan_angle2);

  Math::Vector3D side_normal = radial_dir * cos_angle + cone_axis * sin_angle;
  side_normal = side_normal.normalize();

  return side_normal;
}

extern "C" {
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Cone();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create cone: " << e.what() << std::endl;
    return nullptr;
  }
}
}
