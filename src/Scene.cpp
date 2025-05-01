#include "Scene.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Ray.hpp"
#include "Renderer.hpp"

Raytracer::Scene::Scene(int width, int height, const std::string &outputPath) {
  _path = outputPath;
  _width = width;
  _height = height;
  _window.create(sf::VideoMode(_width, _height), "Raytracer");
  _window.setFramerateLimit(60);
  _window.setVerticalSyncEnabled(true);
  _lastMovement = std::chrono::steady_clock::now();
  init();
}

void Raytracer::Scene::init() {
  _texture.create(_width, _height);
  _image.create(_width, _height);
  _framebuffer.resize(_width * _height);
  _sprite.setTexture(_texture);
}

void Raytracer::Scene::updateImage() {
  for (int y = 0; y < _height; ++y) {
    for (int x = 0; x < _width; ++x) {
      _image.setPixel(x, y, _framebuffer[y * _width + x]);
    }
  }
  _texture.update(_image);
}

void Raytracer::Scene::handleInput(Raytracer::Camera &camera) {
  bool moved = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
    camera.moveForward(0.10f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    camera.moveForward(-0.10f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    camera.moveRight(-0.10f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    camera.moveRight(0.10f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    camera.moveUp(0.10f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
    camera.moveUp(-0.10f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    camera.rotateYaw(-1.0f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    camera.rotateYaw(1.0f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    camera.rotatePitch(-1.0f);
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    camera.rotatePitch(1.0f);
    moved = true;
  }
  if (moved) {
    _lastMovement = std::chrono::steady_clock::now();
    _isHighQuality = false;
  } else {
    auto now = std::chrono::steady_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::seconds>(now - _lastMovement);
    if (duration.count() > _updateQuality && !_isHighQuality) {
      _isHighQuality = true;
    }
  }
}

void Raytracer::Scene::render() {
  Raytracer::Renderer renderer(_width, _height);
  Raytracer::Camera cam;
  cam.setWidth(_width);
  cam.setHeight(_height);
  cam.setZoom(1.0);
  cam.setViewportHeight(2.0);
  cam.setViewportWidth(cam.getViewportHeight());

  while (_window.isOpen()) {
    sf::Event event;
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        _window.close();
      }
    }
    handleInput(cam);
    renderer.renderToBuffer(_framebuffer, cam, _isHighQuality);
    updateImage();
    _window.clear(sf::Color::White);
    _window.draw(_sprite);
    _window.display();
  }
}
