#pragma once

#include <iostream>
#include "Ray.hpp"

namespace Raytracer {

  class IShape {
    public:
      virtual ~IShape() = default;

      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const = 0;

      virtual Math::Vector3D getNormal(const Math::Point3D &hitPoint) const = 0;

      virtual void translate(const Math::Vector3D &offset) = 0;

      virtual const Math::Point3D &getCenter() const = 0; 
      virtual const Math::Vector3D &getColor() const = 0;
      virtual const Math::Vector3D &getAmbient() const = 0;
      virtual const Math::Vector3D &getSpecular() const = 0; 
      virtual const Math::Vector3D &getDiffuse() const = 0;
      virtual double getShininess() const = 0;

      virtual void setCenter(const Math::Point3D &center) = 0;
      virtual void setColor(const Math::Vector3D &color) = 0; 
      virtual void setAmbient(const Math::Vector3D &ambient)= 0;
      virtual void setSpecular(const Math::Vector3D &specular)= 0;
      virtual void setShininess(double shininess) = 0; 
  };

}  // namespace Raytracer
