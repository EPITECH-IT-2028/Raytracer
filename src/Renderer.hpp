#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include "Ray.hpp"
#include "ShapeComposite.hpp"
#include "LightComposite.hpp"
#include <iostream>
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"

namespace Raytracer {
class Renderer {
public:
  Renderer(int width, int height) : _width(width), _height(height) {
  }

  class Renderer {
    public:
      Renderer(int width, int height)
          : _width(width),
            _height(height),
            _scene(ShapeComposite()),
            _light(DirectionalLight(Math::Vector3D(2, -1, -2).normalize())) {
        initScene();
      }

  void writeInFile(const std::string &filename);

      void initScene();

  void createOutputFileName(const std::string &inputFileName);

  const std::string &getOutputFilePath() const { return _outfilePath;}
      void renderToBuffer(std::vector<sf::Color> &framebuffer,
                          Raytracer::Camera &cam, bool isHighQuality);

  void setWidth(const int &width) {_width = width;}
  void setHeight(const int &height) {_height = height;}

private:
  int _width;
  int _height;
  std::string _outfilePath;
  ShapeComposite _scene;
  DirectionalLight _light;
};
}  // namespace Raytracer
