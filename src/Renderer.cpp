#include "Renderer.hpp"
#include <fstream>
#include <iostream>
#include "Camera.hpp"
#include "Cylinder.hpp"
#include "DirectionalLight.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"

void Raytracer::Renderer::writeColor(std::ofstream &file,
                                     const Math::Vector3D &color) {
  file << static_cast<int>(color.x * 255) << " "
       << static_cast<int>(color.y * 255) << " "
       << static_cast<int>(color.z * 255) << "\n";
}

Math::Vector3D Raytracer::Renderer::rayColor(Ray &r, const IShape &shape,
                                             const DirectionalLight &light) {
  auto [t, color, hitShape] = shape.hits(r);

  if (t > 0.0 && hitShape) {
    Math::Point3D hit_point = r.at(t);
    Math::Vector3D normal = hitShape->getNormal(hit_point);
    return light.computeLighting(normal, color);
  }
  Math::Vector3D unit_direction = r.direction.normalize();
  double a = 0.5 * (unit_direction.y + 1.0);
  return Math::Vector3D(1.0, 1.0, 1.0) * (1.0 - a) +
         Math::Vector3D(0.5, 0.7, 1.0) * a;
}

void Raytracer::Renderer::writeHeader(std::ofstream &file) {
  file << "P3\n";
  file << _width << " " << _height << "\n";
  file << "255\n";
}

void Raytracer::Renderer::writeInFile(const std::string &path) {
  Raytracer::Camera cam;
  cam.setWidth(_width);
  cam.setHeight(_height);
  cam.setZoom(1.0);
  cam.setViewportHeight(2.0);
  cam.setViewportWidth(cam.getViewportHeight());

  cam.origin = Math::Point3D(0, 0, 0);

  Raytracer::ShapeComposite group;

  Raytracer::DirectionalLight light1(Math::Vector3D(1, 1, -1).normalize());

  Raytracer::Plane ground(
      Math::Point3D(0, 1, 0),
      Math::Vector3D(0, -1, 0),
      Math::Vector3D(0.8, 0.8, 0.8)
  );
  group.addShape(std::make_shared<Raytracer::Plane>(ground));
  
  std::ofstream file(path);

  writeHeader(file);

  cam.updateView();

  for (double j = 0; j < cam.getWidth(); j++) {
    for (double i = 0; i < cam.getHeight(); i++) {
      Math::Point3D pixel_center =
          cam.getPixel0Location() +
          cam.getPixelDeltaU() * static_cast<float>(i) +
          cam.getPixelDeltaV() * static_cast<float>(j);
      Math::Vector3D ray_direction = (pixel_center - cam.origin).normalize();
      Raytracer::Ray ray(cam.origin, ray_direction);
      Math::Vector3D color = rayColor(ray, group, light1);
      writeColor(file, color);
    }
  }
  file.close();
}
