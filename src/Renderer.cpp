#include "Renderer.hpp"
#include <fstream>
#include <iostream>
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Plane.hpp"
#include "ShapeComposite.hpp"
#include "ParserConfigFile.hpp"
#include "Sphere.hpp"

void Raytracer::Renderer::writeColor(std::ofstream &file,
                                     const Math::Vector3D &color) {
  file << static_cast<int>(color.x * 255) << " "
       << static_cast<int>(color.y * 255) << " "
       << static_cast<int>(color.z * 255) << "\n";
}

Math::Vector3D Raytracer::Renderer::rayColor(Ray &r, const ShapeComposite &shape, const LightComposite &light) {
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

void Raytracer::Renderer::createOutputFileName(const std::string &inputFileName) {
  std::string outputFile = inputFileName;
  outputFile.resize(outputFile.length() - 3);
  outputFile = outputFile + "ppm";
  _outfilePath = outputFile;
}

void Raytracer::Renderer::writeInFile(const std::string &path) {
  Raytracer::Camera cam;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;
  Raytracer::ParserConfigFile parser(path);
  parser.parseConfigFile(cam, sc, lc);
  setWidth(cam.getWidth());
  setHeight(cam.getHeight());
  createOutputFileName(path);
  std::ofstream file(getOutputFilePath());
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
      Math::Vector3D color = rayColor(ray, sc, lc);
      writeColor(file, color);
    }
  }
  file.close();
}
