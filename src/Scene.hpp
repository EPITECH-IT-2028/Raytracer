#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include "Camera.hpp"

namespace Raytracer {
  class Scene {
    public:
      Scene(int width, int height, const std::string &inputFilePath);

      ~Scene() = default;

      void render();

      void init();

      void updateImage();

      void handleInput(Raytracer::Camera &camera);

      void createOutputFileName(const std::string &inputFileName);

      void handleInput(sf::Event &event, Raytracer::Camera &camera);

    private:
      void changeCamQuality();
      bool _cameraMoved = false;
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
      const float _qualityUpdateDelay = 0.5f;
  };
}  // namespace Raytracer
