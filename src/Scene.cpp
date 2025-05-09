#include "Scene.hpp"
#include "Renderer.hpp"
#include <dlfcn.h>
#include <filesystem>

Raytracer::Scene::Scene(int width, int height, const std::string &inputPath)
    : _inputFilePath(inputPath), _width(width), _height(height) {
  _window.create(sf::VideoMode(_width, _height), "Raytracer");
  _window.setFramerateLimit(60);
  _window.setVerticalSyncEnabled(true);
  _lastMovement = std::chrono::steady_clock::now();
  init();
  try {
    parsePlugins();
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] - Failed to parse plugins: " << e.what() << std::endl;
    throw;
  }
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

void Raytracer::Scene::handleInput() {
  const float moveSpeed = 5.0f;
  const float rotateSpeed = 2.0f;

  if (!_window.hasFocus())
    return;

  std::array<sf::Keyboard::Key, 11> movementKeys = {
      sf::Keyboard::Z,      sf::Keyboard::S,     sf::Keyboard::Q,
      sf::Keyboard::D,      sf::Keyboard::Space, sf::Keyboard::LShift,
      sf::Keyboard::RShift, sf::Keyboard::Left,  sf::Keyboard::Right,
      sf::Keyboard::Up,     sf::Keyboard::Down};
  for (const auto &key : movementKeys) {
    if (sf::Keyboard::isKeyPressed(key)) {
      _cameraMoved = true;
      break;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    _camera.moveForward(moveSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    _camera.moveForward(-moveSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    _camera.moveRight(-moveSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    _camera.moveRight(moveSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    _camera.moveUp(moveSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
    _camera.moveUp(-moveSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    _camera.rotateYaw(rotateSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    _camera.rotateYaw(-rotateSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    _camera.rotatePitch(rotateSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    _camera.rotatePitch(-rotateSpeed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    _userQuit = true;
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

void Raytracer::Scene::parsePlugins() {
  for (const auto &entry : std::filesystem::directory_iterator("./plugins")) {
    if (entry.path().extension() == ".so")
      _plugins.push_back(entry.path().string());
    else
     continue;
    void *handler = dlopen(entry.path().c_str(), RTLD_LAZY);
    if (!handler) {
      std::cerr << "[ERROR] - Failed to load plugin: " << entry.path()
                << std::endl;
      continue;
    }
    _pluginHandles.push_back(handler);
  }
}

void Raytracer::Scene::render() {
  Raytracer::Renderer renderer(_width, _height, _inputFilePath, _camera, _plugins);

  while (_window.isOpen()) {
    sf::Event event;
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape))
        _window.close();
    }
    renderer.renderToBuffer(_framebuffer, _camera, _isHighQuality);
    handleInput();
    if (_userQuit)
      return;
    changeCamQuality();
    std::fill(_framebuffer.begin(), _framebuffer.end(), sf::Color::White);
    renderer.renderToBuffer(_framebuffer, _camera, _isHighQuality);
    updateImage();
    _window.clear(sf::Color::White);
    _window.draw(_sprite);
    _window.display();
  }
}

Raytracer::Scene::~Scene() {
  for (auto &handle : _pluginHandles) {
    dlclose(handle);
  }
}
