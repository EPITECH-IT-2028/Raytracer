#pragma once

#include <fstream>
#include <iostream>
#include "DirectionalLight.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

namespace Raytracer {

  class Renderer {
    public:
      Renderer(int width, int height) : _width(width), _height(height) {
      }

      Renderer() : _width(400), _height(400) {
      }

      ~Renderer() = default;

      void writeInFile(const std::string &filename);

      void writeHeader(std::ofstream &file);

      void writeColor(std::ofstream &file, const Math::Vector3D &color);

      Math::Vector3D rayColor(Ray &r, const Sphere &s,
                              const DirectionalLight &light);

    private:
      int _width;
      int _height;
  };

}  // namespace Raytracer
