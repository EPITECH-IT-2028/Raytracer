#include <SFML/Graphics.hpp>
#include <string>
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Scene {
    public:
      Scene(int width, int height);

      ~Scene() = default;

      void render();

      void init();

      void updateImage();

      void handleInput(Raytracer::Camera &camera);

    private:
      std::string _path;
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
  };

}  // namespace Raytracer
