#include "ShapeComposite.hpp"
#include "Vector3D.hpp"

/**
 * @brief Adds a shape to the composite collection.
 * @param shape A shared pointer to the IShape object to be added.
 */
void Raytracer::ShapeComposite::addShape(const std::shared_ptr<IShape> &shape) {
  shapes.push_back(shape);
}

/**
 * @brief Calculates the closest intersection of a ray with the shapes in this composite.
 *
 * Iterates through all shapes in the composite, calls their `hits` method,
 * and returns the information for the closest intersection point found in front of the ray.
 *
 * @param ray The ray to test for intersection.
 * @return A tuple containing:
 *         - double: The distance to the closest hit point (t). If no intersection,
 *                   this value will be 0.0 (or a large initial value if logic changes).
 *         - Math::Vector3D: The color of the shape at the closest hit point.
 *                           (0,0,0) if no hit.
 *         - const IShape*: A pointer to the shape that was hit. Nullptr if no hit.
 */
std::tuple<double, Math::Vector3D, const Raytracer::IShape *>
Raytracer::ShapeComposite::hits(const Raytracer::Ray &ray) const {
  double closestT = 100;  // TODO: Use a more appropriate value
  Math::Vector3D hitColor;
  const IShape *hitShape = nullptr;

  for (const auto &shape : shapes) {
    auto [t, color, s] = shape->hits(ray);
    if (t > 0.0 && t < closestT) {
      closestT = t;
      hitColor = color;
      hitShape = s;
    }
  }
  if (hitShape) {
    return {closestT, hitColor, hitShape};
  }
  return {0.0, Math::Vector3D(0, 0, 0), nullptr};
}
