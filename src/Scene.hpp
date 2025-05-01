#include <SFML/Graphics.hpp>
#include <string>

namespace Raytracer {

  class Scene {
    public:
      Scene(const std::string &);

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
