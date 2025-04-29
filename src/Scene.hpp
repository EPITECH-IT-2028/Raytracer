#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "DirectionalLight.hpp"
#include "Vector3D.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"

namespace Raytracer {

class Scene {
public:
  Scene();

  ~Scene() = default;

  void render();

  void parseBasicsPPM(const std::string &filename);

  void init();

  void getImage();

  Math::Vector3D rayColor(Ray& r, const Sphere& s, const DirectionalLight &light);

private:
  std::string _path;
  int _width;
  int _height;
  sf::RenderWindow _window;
  sf::Texture _texture;
  sf::Sprite _sprite;
  sf::Image _image;
};

} // namespace Raytracer
