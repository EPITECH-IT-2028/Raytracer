#include "Renderer.hpp"
#include <fstream>
#include <iostream>
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Plane.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"

void Raytracer::Renderer::writeColor(std::ofstream &file,
                                     const Math::Vector3D &color) {
  file << static_cast<int>(color.x * 255) << " "
       << static_cast<int>(color.y * 255) << " "
       << static_cast<int>(color.z * 255) << "\n";
}

Math::Vector3D Raytracer::Renderer::rayColor(Ray &r, const Sphere &s,
                                             const DirectionalLight &light) {
  auto [t, color] = s.hits(r);

  if (t > 0.0) {
    Math::Point3D hit_point = r.at(t);
    Math::Vector3D normal = (hit_point - s.getCenter()).normalize();
    return light.computeLighting(normal, s.getColor());
  }
  return Math::Vector3D(0, 0, 0);
}

void Raytracer::Renderer::writeHeader(std::ofstream &file) {
  file << "P3\n";
  file << _width << " " << _height << "\n";
  file << "255\n";
}

void Raytracer::Renderer::writeInFile(const std::string &path) {
  Raytracer::Camera cam;
  cam.setWidth(400);
  cam.setHeight(400);
  cam.setZoom(1.0);
  cam.setViewportHeight(2.0);
  cam.setViewportWidth(cam.getViewportHeight());

  Raytracer::Sphere s(Math::Point3D(0, 0, -1), 0.5, Math::Vector3D(1, 1, 0));
  Raytracer::DirectionalLight light(Math::Vector3D(0, -1, -1).normalize());
  // Raytracer::Plane p(Math::Point3D(0, -0.5, 0), Math::Vector3D(0, 1, 0),
  //                    Math::Vector3D(1, 0, 0));
  Raytracer::ShapeComposite group;
  std::ofstream file(path);

  writeHeader(file);

  auto viewport_u = Math::Vector3D(cam.getViewportWidth(), 0, 0);
  auto viewport_v = Math::Vector3D(0, -cam.getViewportHeight(), 0);
  auto pixel_delta_u = viewport_u / cam.getWidth();
  auto pixel_delta_v = viewport_v / cam.getHeight();
  Math::Point3D viewport_upper_left = cam.origin;
  viewport_upper_left.z = -cam.getZoom();
  viewport_upper_left =
      viewport_upper_left - (viewport_u / 2) - (viewport_v / 2);
  auto pixel00_location =
      viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

  group.addShape(std::make_shared<Raytracer::Sphere>(s));
  // group.addShape(std::make_shared<Raytracer::Plane>(p));
  for (double j = 0; j < cam.getWidth(); j++) {
    for (double i = 0; i < cam.getHeight(); i++) {
      auto pixel_center =
          pixel00_location + (pixel_delta_u * i) + (pixel_delta_v * j);
      Math::Vector3D ray_direction = (pixel_center - cam.origin).normalize();
      Raytracer::Ray ray(cam.origin, ray_direction);
      auto [hit, color] = group.hits(ray);
      if (hit) {
        Math::Vector3D color = rayColor(ray, s, light);
        writeColor(file, color);
      } else {
        writeColor(file, Math::Vector3D(0, 0, 0));
      }
    }
  }
  file.close();
}
