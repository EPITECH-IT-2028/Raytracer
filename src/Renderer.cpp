#include "Renderer.hpp"
#include <memory>
#include "Camera.hpp"
#include "ParserConfigFile.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"
#include <filesystem>
#include <dlfcn.h>

Math::Vector3D Raytracer::Renderer::rayColor(
    Ray &r,                                         
    const ShapeComposite &shape,
    LightComposite &light,
    const Camera& cameraPos
) {
  auto [t, color, hitShape] = shape.hits(r);

  if (t > 0.0 && hitShape) {
    Math::Point3D hitPoint = r.at(t);
    Math::Vector3D normal = hitShape->getNormal(hitPoint);
    Math::Vector3D viewDir = (cameraPos.origin - hitPoint).normalized();
    return light.computeLighting(normal, color, hitPoint, viewDir, shape);
  }
  Math::Vector3D unit_direction = r.direction.normalize();
  double a = 0.5 * (unit_direction.y + 1.0);
  return Math::Vector3D(1.0, 1.0, 1.0) * (1.0 - a) + Math::Vector3D(0.5, 0.7, 1.0) * a;
}

void Raytracer::Renderer::initScene(Camera &camera) {
  ParserConfigFile parser(_inputFilePath, _plugins);
  try {
    parser.parseConfigFile(camera, _shapes, _lights);
  } catch (const std::exception &e) {
    std::cerr << "Error parsing config file: " << e.what() << std::endl;
    throw;
  }
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
      Math::Vector3D color = rayColor(ray, _shapes, _lights, cam);
      sf::Color pixel_color(static_cast<sf::Uint8>(color.x * 255),
                            static_cast<sf::Uint8>(color.y * 255),
                            static_cast<sf::Uint8>(color.z * 255));
      int endY = j + step;
      int endX = i + step;
      for (int blockY = j; blockY < endY; blockY++) {
        for (int blockX = i; blockX < endX; blockX++)
          framebuffer[blockY * _width + blockX] = pixel_color;
      }
    }
  }
}
