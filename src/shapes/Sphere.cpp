#include "Sphere.hpp"
#include <cmath>

double Raytracer::Sphere::hits(const Raytracer::Ray &ray) const {
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius*radius;
    double discriminant = b*b - 4*a*c;
    
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant)) / (2.0*a);
    }
}
