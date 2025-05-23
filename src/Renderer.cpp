#include "Renderer.hpp"
#include <dlfcn.h>
#include <memory>
#include "Camera.hpp"
#include "ParserConfigFile.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"

/**
 * @brief Calculates the color of a ray.
 *
 * @param r The ray to calculate the color for.
 * @param shape The composite shape in the scene.
 * @param light The composite light in the scene.
 * @param cameraPos The camera position.
 * @param depth The current recursion depth for reflections/refractions.
 * @return Math::Vector3D The calculated color of the ray.
 */
Math::Vector3D Raytracer::Renderer::rayColor(Ray &r,
                                             const ShapeComposite &shape,
                                             const LightComposite &light,
                                             const Camera &cameraPos,
                                             int depth) {
  Math::Vector3D unit_direction = r.direction.normalize();
  double a = 0.5 * (unit_direction.y + 1.0);
  Math::Vector3D sky(Math::Vector3D(1.0, 1.0, 1.0) * (1.0 - a) +
                     Math::Vector3D(0.5, 0.7, 1.0) * a);
  if (depth <= 0) {
    return sky;
  }
  auto [t, color, hitShape] = shape.hits(r);

  if (t > 0.0 && hitShape) {
    Math::Point3D hitPoint = r.at(t);
    Math::Vector3D normal = hitShape->getNormal(hitPoint);
    Math::Vector3D viewDir = (cameraPos.origin - hitPoint).normalized();
    Math::Vector3D computeColor =
        light.computeLighting(normal, color, hitPoint, viewDir, shape);
    if (auto material = hitShape->getMaterial()) {
      return material->computeMaterial(
          normal, viewDir, hitPoint, computeColor, shape, light, cameraPos,
          depth - 1,
          [this](Raytracer::Ray &r, const Raytracer::ShapeComposite &s,
                 const Raytracer::LightComposite &l, const Raytracer::Camera &c,
                 int d) { return this->rayColor(r, s, l, c, d); });
    }
    return computeColor;
  }
  return sky;
}

/**
 * @brief Initializes the scene by parsing the configuration file.
 *
 * @param camera The camera object to be initialized.
 */
void Raytracer::Renderer::initScene(Camera &camera) {
  ParserConfigFile parser(_inputFilePath, _plugins);
  parser.parseConfigFile(camera, _shapes, _lights);
}

/**
 * @brief Renders the scene to a framebuffer.
 *
 * @param framebuffer The framebuffer to render to.
 * @param cam The camera used for rendering.
 * @param isHighQuality Whether to render in high quality or not.
 */
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
      Math::Vector3D color = rayColor(ray, _shapes, _lights, cam, _maxDepth);
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
