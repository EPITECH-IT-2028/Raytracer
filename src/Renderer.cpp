#include "Renderer.hpp"
#include <fstream>
#include <iostream>
#include "Camera.hpp"
#include "Plane.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"

void Raytracer::Renderer::writeColor(std::ofstream &file,
                                     const Math::Vector3D &color) {
  file << static_cast<int>(color.x * 255) << " "
       << static_cast<int>(color.y * 255) << " "
       << static_cast<int>(color.z * 255) << "\n";
}

void Raytracer::Renderer::writeHeader(std::ofstream &file) {
  file << "P3\n";
  file << _width << " " << _height << "\n";
  file << "255\n";
}

void Raytracer::Renderer::writeInFile(const std::string &path) {
  Raytracer::Camera cam;
  Raytracer::Sphere s(Math::Point3D(0, 0, -1), 0.5, Math::Vector3D(1, 1, 0));
  Raytracer::Plane p(Math::Point3D(0, -0.5, 0), Math::Vector3D(0, 1, 0),
                     Math::Vector3D(1, 0, 0));
  Raytracer::ShapeComposite group;
  std::ofstream file(path);

  writeHeader(file);
  group.addShape(std::make_shared<Raytracer::Sphere>(s));
  group.addShape(std::make_shared<Raytracer::Plane>(p));
  for (double j = 0; j < _width; j++) {
    for (double i = 0; i < _height; i++) {
      double u = i / static_cast<double>(_width);
      double v = j / static_cast<double>(_height);
      Raytracer::Ray ray = cam.ray(u, v);
      auto [hit, color] = group.hits(ray);
      if (hit) {
        writeColor(file, color);
      } else {
        writeColor(file, Math::Vector3D(0, 0, 0));
      }
    }
  }
  file.close();
}
