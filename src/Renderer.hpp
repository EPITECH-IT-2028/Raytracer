#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
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
      Renderer(int width, int height)
          : _width(width),
            _height(height),
            _scene(ShapeComposite()),
            _light(DirectionalLight(Math::Vector3D(2, -1, -2).normalize())) {
        initScene();
      }

      ~Renderer() = default;

      void initScene();

      Math::Vector3D rayColor(Ray &r, const IShape &s,
                              const DirectionalLight &light);

      void renderToBuffer(std::vector<sf::Color> &framebuffer,
                          Raytracer::Camera &cam);

    private:
      int _width;
      int _height;
      ShapeComposite _scene;
      DirectionalLight _light;
  };

}  // namespace Raytracer
