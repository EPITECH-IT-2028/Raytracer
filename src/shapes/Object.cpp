#include <tuple>
#include <vector>
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"
#include "Object.hpp"
#include <iostream>

/**
 * @brief Calculates the intersection of a ray with the object's faces (triangles).
 *
 * Iterates through each face of the object, performing a ray-triangle intersection test
 * (Möller–Trumbore algorithm) for each. It returns the closest valid intersection.
 *
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance from the ray's origin to the closest intersection point (t).
 *                   Returns 0.0 if there is no hit.
 *         - Math::Vector3D: The diffuse color of the material of the hit face.
 *                           Defaults to white (1,1,1) if material is not found.
 *                           Returns (0,0,0) if no hit.
 *         - const Raytracer::IShape*: A pointer to this object.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *> Raytracer::Object::hits(
    const Raytracer::Ray &ray) const {
    double eps = 0.0001;
    double closest_t = 0.0;
    Math::Vector3D closest_color = {0.0, 0.0, 0.0};
    bool hit = false;

    for (const auto &face : _faces) {
      Math::Point3D v0 = _vertices[face.vertex[0]];
      Math::Point3D v1 = _vertices[face.vertex[1]];
      Math::Point3D v2 = _vertices[face.vertex[2]];

      Math::Vector3D edge1 = v1 - v0;
      Math::Vector3D edge2 = v2 - v0;
      Math::Vector3D h = Math::cross(ray.direction, edge2);
      double a = edge1.dot(h);

      if (a > -eps && a < eps)
        continue;

      double f = 1.0 / a;
      Math::Vector3D s = ray.origin - v0;
      double u = f * s.dot(h);

      if (u < 0.0 || u > 1.0)
        continue;

      Math::Vector3D q = Math::cross(s, edge1);
      double v = f * ray.direction.dot(q);

      if (v < 0.0 || u + v > 1.0)
        continue;

      double t = f * edge2.dot(q);

      if (t > eps && (t < closest_t || closest_t == 0.0)) {
        closest_t = t;
        hit = true;

        if (_materials.find(face.material_name) != _materials.end()) {
          const Mtl &material = _materials.at(face.material_name);
          closest_color = material.diffuse;
        } else {
          closest_color = {1.0, 1.0, 1.0};
        }
      }
    }
    if (hit)
      return {closest_t, closest_color, this};
    else
      return {0.0, Math::Vector3D(0.0, 0.0, 0.0), this};
}

extern "C" {
/**
 * @brief Factory function to create a new Object instance.
 *
 * This function is typically used by a plugin system to instantiate shape objects
 * that are loaded from OBJ files.
 * @return Raytracer::IShape* A pointer to the newly created Object, or nullptr on failure.
 */
Raytracer::IShape *addShape() {
  try {
    return new Raytracer::Object();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return nullptr;
  }
}
}
