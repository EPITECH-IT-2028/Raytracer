#include "Scene.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Ray.hpp"

Raytracer::Scene::Scene() {
  _path = "./output.ppm";
  parseBasicsPPM(_path);
  _window.create(sf::VideoMode(_width, _height), "Raytracer");
  _window.setFramerateLimit(60);
  _window.setVerticalSyncEnabled(true);
}

void Raytracer::Scene::parseBasicsPPM(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }
  std::string line;
  std::getline(file, line);
  if (line != "P3") {
    std::cerr << "Invalid PPM file format" << std::endl;
    return;
  }
  std::getline(file, line);
  std::istringstream iss(line);
  iss >> _width >> _height;
}

void Raytracer::Scene::init() {
  _texture.create(_width, _height);
  _image.create(_width, _height);
}

void Raytracer::Scene::getImage() {
  std::ifstream file(_path);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << _path << std::endl;
    return;
  }
  std::string line;
  std::getline(file, line);
  if (line != "P3") {
    std::cerr << "Invalid PPM file format" << std::endl;
    return;
  }
  for (int y = 0; y < _height; ++y) {
    for (int x = 0; x < _width; ++x) {
      int r, g, b;
      file >> r >> g >> b;
      _image.setPixel(x, y, sf::Color(r, g, b));
    }
  }
  file.close();
}

void Raytracer::Scene::render() {
  getImage();
  _texture.update(_image);
  _sprite.setTexture(_texture);
  while (_window.isOpen()) {
    sf::Event event;
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        _window.close();
      }
    }
    _window.clear(sf::Color::White);
    _window.draw(_sprite);
    _window.display();
  }
}
