#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"
#include "Triangle.hpp"
#include <iostream>

std::tuple<double, Math::Vector3D, const Raytracer::IShape *> Raytracer::Triangle::hits(
    const Raytracer::Ray &ray) const {
    double eps = 0.0001;

    Math::Vector3D edge1 = _p2 - _p1;
    Math::Vector3D edge2 = _p3 - _p1;
    Math::Vector3D h = Math::cross(ray.direction, edge2);
    double a = edge1.dot(h);

    if (a > -eps && a < eps)
      return {0.0, _color, this};

    double f = 1.0 / a;
    Math::Vector3D s = ray.origin - _p1;
    double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
      return {0.0, _color, this};

    Math::Vector3D q = Math::cross(s, edge1);
    double v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
      return {0.0, _color, this};

    double t = f * edge2.dot(q);

    if (t > eps)
      return {t, _color, this};
    return {0.0, _color, this};
}

extern "C" {
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Triangle();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return nullptr;
  }
}
}