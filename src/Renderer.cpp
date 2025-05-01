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

void Raytracer::Renderer::initScene() {
  auto sphere1 = std::make_shared<Raytracer::Sphere>(
      Math::Point3D(0, 0, -1), 0.5, Math::Vector3D(1, 0, 0));
  Raytracer::Sphere s2(Math::Point3D(-1, -0.3, -2.0), 0.5,
                       Math::Vector3D(0, 0, 1));
  Raytracer::Cylinder c1(Math::Point3D(1, 0, -1), 0.5, 1,
                         Math::Vector3D(0, 1, 0), Math::Vector3D(1, 0, 0));

  _scene.addShape(sphere1);
  _scene.addShape(std::make_shared<Raytracer::Sphere>(s2));
  _scene.addShape(std::make_shared<Raytracer::Cylinder>(c1));
}

void Raytracer::Renderer::renderToBuffer(std::vector<sf::Color> &framebuffer,
                                         Raytracer::Camera &cam,
                                         bool isHighQuality) {
  cam.updateView();

  int step = isHighQuality ? 1 : 5;

  for (double j = 0; j < _height; j += step) {
    for (double i = 0; i < _width; i += step) {
      Math::Point3D pixel_center =
          cam.getPixel0Location() +
          cam.getPixelDeltaU() * static_cast<float>(i) +
          cam.getPixelDeltaV() * static_cast<float>(j);
      Math::Vector3D ray_direction = (pixel_center - cam.origin).normalize();
      Raytracer::Ray ray(cam.origin, ray_direction);
      Math::Vector3D color = rayColor(ray, _scene, _light);
      sf::Color pixel_color(static_cast<sf::Uint8>(color.x * 255),
                            static_cast<sf::Uint8>(color.y * 255),
                            static_cast<sf::Uint8>(color.z * 255));
      framebuffer[j * _width + i] = pixel_color;
    }
  }
}
