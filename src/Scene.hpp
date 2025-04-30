#include <SFML/Graphics.hpp>
#include <string>
#include "DirectionalLight.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vector3D.hpp"

namespace Raytracer {

  class Scene {
    public:
      Scene();

      ~Scene() = default;

      void render();

      void parseBasicsPPM(const std::string &filename);

      void init();

      void getImage();

    private:
      std::string _path;
      int _width;
      int _height;
      sf::RenderWindow _window;
      sf::Texture _texture;
      sf::Sprite _sprite;
      sf::Image _image;
  };

}  // namespace Raytracer
