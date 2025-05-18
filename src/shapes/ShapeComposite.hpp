#pragma once

#include <memory>
#include <vector>
#include "AShape.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  /**
   * @brief A composite shape that groups multiple IShape objects.
   *
   * This class inherits from AShape and allows treating a collection of shapes
   * as a single entity. It delegates operations like ray intersection testing
   * and transformations to its child shapes.
   */
  class ShapeComposite : public AShape {
    public:
      /**
       * @brief Default constructor for ShapeComposite.
       */
      ShapeComposite() = default;

      /**
       * @brief Adds a shape to the composite.
       * @param shape A shared pointer to the IShape object to add.
       */
      void addShape(const std::shared_ptr<IShape> &shape);

      /**
       * @brief Calculates the closest intersection of a ray with any shape in the composite.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the closest hit point, the color of the hit shape, and a pointer to the hit shape itself. If no shape is hit, returns a very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Gets the normal vector at a given point.
       * @note This implementation is a placeholder and returns a zero vector.
       *       A proper implementation would require identifying the specific child shape
       *       that was hit and calling its getNormal method.
       * @param point The point on a shape's surface.
       * @return Math::Vector3D The normal vector (currently a zero vector).
       */
      Math::Vector3D getNormal(const Math::Point3D &point) const override {
        (void)point;
        return Math::Vector3D(0, 0, 0);
      }

      /**
       * @brief Gets the collection of shapes in this composite.
       * @return const std::vector<std::shared_ptr<IShape>>& A const reference to the vector of child shapes.
       */
      const std::vector<std::shared_ptr<IShape>> &getShapes() const {
        return shapes;
      };

      /**
       * @brief Translates all shapes within the composite by a given offset.
       * @param offset The vector by which to translate the shapes.
       */
      void translate(const Math::Vector3D &offset) override {
        for (const auto &shape : shapes)
          shape->translate(offset);
      }

    private:
      std::vector<std::shared_ptr<IShape>> shapes; ///< Vector of shared pointers to IShape objects.
  };

}  // namespace Raytracer
