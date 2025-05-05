#include "Scene.hpp"
#include "Renderer.hpp"

Raytracer::Scene::Scene(int width, int height, const std::string &inputPath)
    : _inputFilePath(inputPath), _width(width), _height(height) {
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

void Raytracer::Scene::handleInput(sf::Event &event,
                                   Raytracer::Camera &camera) {
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
      case sf::Keyboard::Z:
        camera.moveForward(5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::S:
        camera.moveForward(-5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::Q:
        camera.moveRight(-5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::D:
        camera.moveRight(5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::Space:
        camera.moveUp(5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::LShift:
        camera.moveUp(-5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::Left:
        camera.rotateYaw(5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::Right:
        camera.rotateYaw(-5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::Up:
        camera.rotatePitch(5.0f);
        _cameraMoved = true;
        break;
      case sf::Keyboard::Down:
        camera.rotatePitch(-5.0f);
        _cameraMoved = true;
        break;
      default:
        break;
    }
  }
}

void Raytracer::Scene::changeCamQuality() {
  if (_cameraMoved) {
    _lastMovement = std::chrono::steady_clock::now();
    _isHighQuality = false;
  } else {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = now - _lastMovement;
    if (duration.count() > _qualityUpdateDelay.count() && !_isHighQuality) {
      _isHighQuality = true;
    }
  }
  _cameraMoved = false;
}

void Raytracer::Scene::render() {
  Raytracer::Camera cam;
  Raytracer::Renderer renderer(_width, _height, _inputFilePath, cam);

  while (_window.isOpen()) {
    sf::Event event;
    while (_window.pollEvent(event)) {
      handleInput(event, cam);
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape))
        _window.close();
    }
    changeCamQuality();
    std::fill(_framebuffer.begin(), _framebuffer.end(), sf::Color::White);
    renderer.renderToBuffer(_framebuffer, cam, _isHighQuality);
    updateImage();
    _window.clear(sf::Color::White);
    _window.draw(_sprite);
    _window.display();
  }
}
