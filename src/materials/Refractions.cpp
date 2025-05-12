#include "Refractions.hpp"
#include "Vector3D.hpp"
#include <cmath>
#include <iostream>

Math::Vector3D Raytracer::Refractions::computeMaterial(
    const Math::Vector3D &normal, const Math::Vector3D &viewDir,
    const Math::Point3D &hitPoint, const Math::Vector3D &color,
    const Raytracer::ShapeComposite &shapes,
    const Raytracer::LightComposite &lights, const Raytracer::Camera &camera,
    int depth, RayColorFunc rayColorFunc) const {
    if (depth <= 0) {
        return color;
    }
    const float n1 = 1.0f;
    const float n2 = 1.5f;
    Math::Vector3D incidentDir = -viewDir;
    float cosI_initial = incidentDir.dot(normal);
    Math::Vector3D refractiveNormal;
    float refractionRatio;
    if (cosI_initial < 0) {
        refractiveNormal = normal;
        refractionRatio = n1 / n2;
    } else {
        refractiveNormal = -normal;
        refractionRatio = n2 / n1;
    }
    float cosI = incidentDir.dot(refractiveNormal);
    float cosT2 = 1.0f - (refractionRatio * refractionRatio) * (1.0f - cosI * cosI);
    if (cosT2 < 0.0f) {
        Math::Vector3D reflectedDir = incidentDir - refractiveNormal * 2.0f * cosI;
        reflectedDir.normalize();
        Raytracer::Ray reflectedRay(hitPoint + reflectedDir * 0.001f, reflectedDir);
        return rayColorFunc(reflectedRay, shapes, lights, camera, depth - 1);
    }
    float cosT = std::sqrt(cosT2);
    Math::Vector3D refractedDir = incidentDir * refractionRatio + refractiveNormal * (refractionRatio * cosI - cosT);
    refractedDir.normalize();
    Raytracer::Ray refractedRay(hitPoint + refractedDir * 0.001f, refractedDir);
    Math::Vector3D refractedColor = rayColorFunc(refractedRay, shapes, lights, camera, depth - 1);
    return color * 0.1f + refractedColor * 0.9f;
}

extern "C" {
Raytracer::IMaterials *addMaterial() {
  try {
    return new Raytracer::Refractions();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to create Refractions material: " << e.what()
              << std::endl;
    return nullptr;
  }
}
}
