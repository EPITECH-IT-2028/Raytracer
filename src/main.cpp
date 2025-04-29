#include "Camera.hpp"
#include "Plane.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Rectangle.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Vector3D.hpp"
#include <fstream>
#include <iostream>

void writeColor(std::ofstream &file, const Math::Vector3D &color) {
  file << static_cast<int>(color.x * 255) << " "
       << static_cast<int>(color.y * 255) << " "
       << static_cast<int>(color.z * 255) << "\n";
}

int main(void) {
  Raytracer::Camera cam;
  Raytracer::Sphere s(Math::Point3D(0, 0, -1), 0.5);
  Raytracer::Plane p(Math::Point3D(0, -0.5, 0), Math::Vector3D(0, 1, 0));

  std::ofstream file("output.ppm");
  int width = 400;
  int height = 400;
  file << "P3\n";
  file << width << " " << height << "\n";
  file << "255\n";

  for (double j = 0; j < width; j++) {
    for (double i = 0; i < height; i++) {
      double u = i / 400.0;
      double v = j / 400.0;
      Raytracer::Ray ray = cam.ray(u, v);
      if (s.hits(ray)) {
        writeColor(file, Math::Vector3D(1, 0, 0));
      } else if (p.hits(ray)) {
        writeColor(file, Math::Vector3D(0, 1, 0));
      } else {
        writeColor(file, Math::Vector3D(0, 0, 1));
      }
    }
  }
  file.close();
  Raytracer::Scene scene;
  scene.init();
  scene.render();
  return 0;
}
