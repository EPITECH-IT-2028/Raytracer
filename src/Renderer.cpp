#include "Renderer.hpp"
#include <dlfcn.h>
#include <memory>
#include "Camera.hpp"
#include "ParserConfigFile.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"

/**
 * @brief Computes the color seen along a ray in the scene, supporting recursive material shading and sky gradient.
 *
 * Calculates the visible color for a given ray by determining intersections with scene shapes, applying lighting and material effects, and supporting recursive shading up to a specified depth. Returns a sky color gradient if no intersection occurs or the recursion depth limit is reached.
 *
 * @param r The ray for which to compute the color.
 * @param shape The collection of scene shapes to test for intersections.
 * @param light The scene's lighting configuration.
 * @param cameraPos The camera's position and orientation.
 * @param depth The remaining recursion depth for material effects.
 * @return Math::Vector3D The computed color as a 3D vector.
 */
Math::Vector3D Raytracer::Renderer::rayColor(Ray &r,
                                             const ShapeComposite &shape,
                                             LightComposite &light,
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
                 Raytracer::LightComposite &l, const Raytracer::Camera &c,
                 int d) { return this->rayColor(r, s, l, c, d); });
    }
    return computeColor;
  }
  return sky;
}

/**
 * @brief Initializes the scene by parsing the configuration file and loading shapes and lights.
 *
 * Populates the provided camera, as well as the renderer's shape and light composites, using the configuration file and available plugins.
 *
 * @param camera Reference to the camera object to be initialized.
 *
 * @throws std::exception If parsing the configuration file fails.
 */
void Raytracer::Renderer::initScene(Camera &camera) {
  ParserConfigFile parser(_inputFilePath, _plugins);
  try {
    parser.parseConfigFile(camera, _shapes, _lights);
  } catch (const std::exception &e) {
    std::cerr << "Error parsing config file: " << e.what() << std::endl;
    throw;
  }
}

/**
 * @brief Renders the scene into the provided framebuffer using ray tracing.
 *
 * Fills the framebuffer with colors computed by tracing rays from the camera through each pixel. The rendering quality and pixel stepping are determined by the `isHighQuality` flag, with block rendering used for lower quality to improve performance.
 *
 * @param framebuffer Output vector to be filled with pixel colors.
 * @param cam Camera used to generate rays for each pixel.
 * @param isHighQuality If true, renders every pixel; otherwise, renders in larger blocks for faster, lower-quality output.
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
      Math::Vector3D color = rayColor(ray, _shapes, _lights, cam, 5);
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
