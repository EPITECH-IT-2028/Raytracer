#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "LightComposite.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"

namespace Raytracer {
  class Renderer {
    public:
      Renderer() = default;
      Renderer(int width, int height, const std::string &inputFilePath,
               Camera &cam)
          : _width(width),
            _height(height),
            _inputFilePath(inputFilePath),
            _shapes(ShapeComposite()),
            _lights(LightComposite()) {
        initScene(cam);
      }

      void writeInFile(const std::string &filename);

      void initScene(Camera &camera);
      Math::Vector3D rayColor(Ray &r, const ShapeComposite &s,
                              const LightComposite &light);

      const std::string &getInputFilePath() const {
        return _inputFilePath;
      }

      void renderToBuffer(std::vector<sf::Color> &framebuffer,
                          Raytracer::Camera &cam, bool isHighQuality);

      void setWidth(int width) {
        _width = width;
      }
      void setHeight(int height) {
        _height = height;
      }

    private:
      int _width;
      int _height;
      std::string _inputFilePath;
      ShapeComposite _shapes;
      LightComposite _lights;
  };
}  // namespace Raytracer
