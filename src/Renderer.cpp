#include "Renderer.hpp"
#include "Camera.hpp"
#include "ParserConfigFile.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"

Math::Vector3D Raytracer::Renderer::rayColor(Ray &r,
                                             const ShapeComposite &shape,
                                             const LightComposite &light) {
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

void Raytracer::Renderer::initScene(Camera &camera) {
  ParserConfigFile parser(_inputFilePath);
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
      Math::Vector3D color = rayColor(ray, _shapes, _lights);
      sf::Color pixel_color(static_cast<sf::Uint8>(color.x * 255),
                            static_cast<sf::Uint8>(color.y * 255),
                            static_cast<sf::Uint8>(color.z * 255));
      framebuffer[j * _width + i] = pixel_color;
    }
  }
}
