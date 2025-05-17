#include "Scene.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include "Renderer.hpp"
#include "exceptions/RaytracerException.hpp"

#define EXTENSION_LENGTH 4


Raytracer::Scene::Scene(int width, int height, const std::string &inputPath)
    : _inputFilePath(inputPath), _width(width), _height(height) {
  _window.create(sf::VideoMode(_width, _height), "Raytracer");
  _window.setFramerateLimit(60);
  _window.setVerticalSyncEnabled(true);
  _lastMovement = std::chrono::steady_clock::now();
  _ftime = std::filesystem::last_write_time(inputPath);
  init();
  try {
    parsePlugins();
    _renderer = std::make_unique<Raytracer::Renderer>(
        _width, _height, _inputFilePath, _camera, _plugins);
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

void Raytracer::Scene::writeColor(std::ofstream &file, sf::Color color) {
  file << std::to_string(color.r) << " " << std::to_string(color.g) << " "
       << std::to_string(color.b) << "\n";
}

void Raytracer::Scene::createOutputFileName() {
  if (!_inputFilePath.ends_with(".cfg")) {
    throw RaytracerError("File must have the following extension: \"*.cfg");
  }
  std::size_t indexCompleteFileName = 0;
  std::size_t counterScreenshotFileName = 0;
  std::string outputFileName;

  for (std::size_t i = _inputFilePath.length(); i > 0; i--) {
    if (_inputFilePath[i] == '/') {
      indexCompleteFileName = i;
      break;
    }
  }
  outputFileName = _inputFilePath.substr(
      indexCompleteFileName,
      _inputFilePath.length() - indexCompleteFileName - EXTENSION_LENGTH);
  for (const auto &entry :
       std::filesystem::directory_iterator("./screenshots/")) {
    std::string path = entry.path();
    if (path.find(outputFileName)) {
      counterScreenshotFileName++;
    }
  }
  if (!std::filesystem::exists("./screenshots")) {
    std::filesystem::create_directory("./screenshots");
  }
  outputFileName =
      "./screenshots/" + outputFileName +
      (counterScreenshotFileName >= 1
           ? ("(" + std::to_string(counterScreenshotFileName) + ")" + ".ppm")
           : ".ppm");
  _outputFilePath = outputFileName;
}


void Raytracer::Scene::createPPMFile() {
  createOutputFileName();
  std::ofstream outputFile(_outputFilePath);
  if (!outputFile.is_open()) {
    throw RaytracerError("Error while opening the ouputfile.");
  }
  outputFile << "P3\n";
  outputFile << _width << " " << _height << "\n";
  outputFile << "255\n";
  for (const auto& color : _framebuffer) {
    writeColor(outputFile, color);
  }
  outputFile.close();
}

void Raytracer::Scene::checkFileChange() {
  std::filesystem::file_time_type updateTime = std::filesystem::last_write_time(_inputFilePath);

  if (updateTime == _ftime) {
    return;
  }
  _ftime = updateTime;
  try {
    _renderer = std::make_unique<Raytracer::Renderer>(_width, _height, _inputFilePath, _camera, _plugins);
  } catch (const Raytracer::ParseError &e) {
    std::cerr << "[ERROR] - Failed to update renderer: " << e.what() << std::endl;
  } catch (const Raytracer::RaytracerError &e) {
    std::cerr << "[ERROR] - Failed to update renderer: " << e.what() << std::endl;
  }
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
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
    createPPMFile();
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
      std::cerr << "[WARNING] - Failed to load plugin: " << entry.path()
                << std::endl;
      continue;
    }
    _pluginHandles.push_back(handler);
  }
}

void Raytracer::Scene::render() {
  createPPMFile();

  while (_window.isOpen()) {
    sf::Event event;
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape))
        _window.close();
    }
    checkFileChange();
    handleInput();
    if (_userQuit)
      return;
    changeCamQuality();
    std::fill(_framebuffer.begin(), _framebuffer.end(), sf::Color::White);
    _renderer->renderToBuffer(_framebuffer, _camera, _isHighQuality);
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
