#pragma once

#include "IShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class AShape : public IShape {
    public:
      virtual ~AShape() = default;

      virtual std::tuple<double, Math::Vector3D, const IShape *> hits(
          const Raytracer::Ray &ray) const override = 0;

      virtual Math::Vector3D getNormal(const Math::Point3D &hitPoint) const override = 0;

      virtual void translate(const Math::Vector3D &offset) override {
        _center = _center + offset;
      }

      const Math::Point3D &getCenter() const override { return _center;}
      const Math::Vector3D &getColor() const override { return _color;}
      const Math::Vector3D &getAmbient() const override { return _ambient;}
      const Math::Vector3D &getSpecular() const override  { return _specular;}
      const Math::Vector3D &getDiffuse() const override  { return _diffuse;}
      double getShininess() const override { return _shininess;}
      

      void setCenter(const Math::Point3D &center) override { _center = center;}
      void setColor(const Math::Vector3D &color) override { _color = color;}
      void setAmbient(const Math::Vector3D &ambient) override { _ambient = ambient;}
      void setSpecular(const Math::Vector3D &specular) override { _specular = specular;}
      void setShininess(double shininess) override  { _shininess = shininess;}

    protected:
      Math::Point3D _center;
      Math::Vector3D _color;
      Math::Vector3D _diffuse = Math::Vector3D(1.0, 1.0, 1.0);
      Math::Vector3D _ambient = Math::Vector3D(1.,1.,1.);
      Math::Vector3D _specular = Math::Vector3D(0.5,0.5,0.5);
      double _shininess = 32.;
  };

}  // namespace Raytracer
