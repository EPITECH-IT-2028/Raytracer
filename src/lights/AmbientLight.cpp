#include "AmbientLight.hpp"

Math::Vector3D Raytracer::AmbientLight::computeLighting(const Math::Vector3D& normal, const IShape *hitShape, const Math::Vector3D &viewDir) const {
    (void)normal;
    (void)viewDir;
    
    if (hitShape == nullptr) {
        throw std::runtime_error("HitShape was nullptr");
    }
    return hitShape->getColor() * (_color * _intensity);
}
