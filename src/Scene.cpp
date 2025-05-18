#include "Scene.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include "Renderer.hpp"
#include "exceptions/RaytracerException.hpp"

#define EXTENSION_LENGTH 4

/**
 * @brief Constructor for the Scene class.
 * @param width The width of the scene/window.
 * @param height The height of the scene/window.
 * @param inputPath The path to the scene configuration file.
 * @throws RaytracerError if plugin parsing fails.
 */
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

/**
 * @brief Initializes the scene components.
 * This includes creating the texture, image, framebuffer, and sprite.
 */
void Raytracer::Scene::init() {
  _texture.create(_width, _height);
  _image.create(_width, _height);
  _framebuffer.resize(_width * _height);
  _sprite.setTexture(_texture);
}

/**
 * @brief Updates the image buffer with the rendered scene from the framebuffer.
 */
void Raytracer::Scene::updateImage() {
  for (int y = 0; y < _height; ++y) {
    for (int x = 0; x < _width; ++x) {
      _image.setPixel(x, y, _framebuffer[y * _width + x]);
    }
  }
  _texture.update(_image);
}

/**
 * @brief Writes a color to the output file stream in PPM format.
 * @param file The output file stream.
 * @param color The SFML color to write.
 */
void Raytracer::Scene::writeColor(std::ofstream &file, sf::Color color) {
  file << std::to_string(color.r) << " " << std::to_string(color.g) << " "
       << std::to_string(color.b) << "\n";
}

/**
 * @brief Creates the output file name for the PPM image.
 * The name is based on the input configuration file name and stored in _outputFilePath.
 * It also handles creating the "screenshots" directory if it doesn't exist
 * and appends a counter to the filename if a file with the same name already exists.
 * @throws RaytracerError if the input file does not have a ".cfg" extension.
 */
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

/**
 * @brief Creates the output PPM file with the rendered image.
 * This method calls createOutputFileName() to determine the file path,
 * then writes the framebuffer data to the PPM file.
 * @throws RaytracerError if the output file cannot be opened.
 */
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

/**
 * @brief Handles user input for camera movement and other interactions.
 * This includes moving/rotating the camera, taking screenshots (Y key),
 * and quitting (Escape key).
 */
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

/**
 * @brief Manages camera rendering quality based on movement.
 * If the camera has moved, it switches to low quality. If the camera
 * has been stationary for a certain duration (_qualityUpdateDelay),
 * it switches back to high quality.
 */
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

/**
 * @brief Parses and loads plugins from the "./plugins" directory.
 * It looks for files with the ".so" extension and attempts to load them.
 * Warnings are printed if a plugin fails to load.
 */
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

/**
 * @brief Renders the scene and handles the main event loop.
 * Initializes the renderer, renders an initial frame, creates a PPM file,
 * and then enters the main loop. In the loop, it polls for events,
 * handles input, updates camera quality, re-renders the scene,
 * updates the image, and displays it.
 */
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

/**
 * @brief Destructor for the Scene class.
 * Closes any loaded plugin handles.
 */
Raytracer::Scene::~Scene() {
  for (auto &handle : _pluginHandles) {
    dlclose(handle);
  }
}
