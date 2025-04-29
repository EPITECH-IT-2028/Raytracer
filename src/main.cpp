#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Point3D.hpp"
#include "Rectangle.hpp"
#include "Scene.hpp"
#include "ShapeComposite.hpp"
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
  cam.setWidth(400);
  cam.setHeight(400);
  cam.setZoom(1.);
  cam.setViewportHeight(2.0);
  cam.setViewportWidth(cam.getViewportHeight());
  
  Raytracer::Sphere sphere(Math::Point3D(0, 0, -1), 0.5, {0.9, 0.5, 0.2});
  Raytracer::DirectionalLight light(Math::Vector3D(0, -1, -1).normalize());
  
  auto viewport_u = Math::Vector3D(cam.getViewportWidth(), 0, 0);
  auto viewport_v = Math::Vector3D(0, -cam.getViewportHeight(), 0);
  auto pixel_delta_u = viewport_u / cam.getWidth();
  auto pixel_delta_v = viewport_v / cam.getHeight();
  Math::Point3D viewport_upper_left = cam.origin;
  viewport_upper_left.z -= cam.getZoom();
  viewport_upper_left = viewport_upper_left - viewport_u/2 - viewport_v/2;
  auto pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
  Raytracer::Scene scene;

  std::ofstream file("output.ppm");
  file << "P3\n";
  file << cam.getWidth() << " " << cam.getHeight() << "\n";
  file << "255\n";
  
  for (std::size_t j = 0; j < cam.getHeight(); j++) {
    for (std::size_t i = 0; i < cam.getWidth(); i++) {
      auto pixel_center = pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j);
      Math::Vector3D ray_direction = pixel_center - cam.origin;
      ray_direction.normalize();
      Raytracer::Ray ray(cam.origin, ray_direction);
      if (sphere.hits(ray) > 0) {
        Math::Vector3D color = scene.rayColor(ray, sphere, light); // Simple red color for now
        writeColor(file, color);
      } else {
        writeColor(file, {0,0,0});
      }
    }
  }
  file.close();
  scene.init();
  scene.render();
  return 0;
}
