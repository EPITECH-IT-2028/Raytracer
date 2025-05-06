#pragma once

#include "AShape.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include "Ray.hpp"
#include <tuple>

namespace Raytracer {

class Cone : public AShape {
public:
    Cone(const Math::Point3D &center, double radius, double height,
             const Math::Vector3D &normal, const Math::Vector3D &color)
        : _center(center), _normal(normal),
          _radius(radius), _height(height), _color(color) {}

    Cone()
        : _center(Math::Point3D(0, 0, 0)),
          _normal(Math::Vector3D(0, 1, 0)),
          _radius(1.0),
          _height(2.0),
          _color(Math::Vector3D(0, 0, 1))
          {}

    ~Cone() = default;

    std::tuple<double, Math::Vector3D, const IShape *> hits(
        const Raytracer::Ray &ray) const override;

    Math::Vector3D getNormal(const Math::Point3D &point) const override {
        Math::Vector3D oc = point - _center;
        double oc_dot_normal = oc.dot(_normal);
        Math::Vector3D dir_perp = oc - (_normal * oc_dot_normal);
        return dir_perp.normalize();
    }
    

private:
    Math::Point3D _center;
    Math::Vector3D _normal;
    double _radius;
    double _height;
    double _angle;
    Math::Vector3D _color;
};

} // namespace Raytracer