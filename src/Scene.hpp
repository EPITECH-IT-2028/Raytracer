#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <vector>
#include "Camera.hpp"

namespace Raytracer {
  class Scene {
    public:
      Scene(int width, int height, const std::string &inputFilePath);

  ~Scene();

      void render();

      void init();

      void updateImage();

      void handleInput();

  void createOutputFileName(const std::string &inputFileName);

  void parsePlugins();

private:
  std::string _inputFilePath;
  int _width;
  int _height;
  sf::RenderWindow _window;
  sf::Texture _texture;
  sf::Sprite _sprite;
  sf::Image _image;
  std::vector<sf::Color> _framebuffer;
  std::chrono::time_point<std::chrono::steady_clock> _lastMovement;
  bool _isHighQuality = true;
  const float _updateQuality = 0.5f;
  std::vector<std::string> _plugins;
  std::vector<void *> _pluginHandles;
  Raytracer::Camera _camera;
  void changeCamQuality();
  bool _cameraMoved = false;
  const std::chrono::duration<float> _qualityUpdateDelay{0.5f};
  bool _userQuit = false;
  };
}  // namespace Raytracer
