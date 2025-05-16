#include <SFML/Graphics.hpp>
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>
#include "Camera.hpp"
#include "Renderer.hpp"

namespace Raytracer {
  class Scene {
    public:
      Scene(int width, int height, const std::string &inputFilePath);

      ~Scene();

      void render();

      void init();

      void updateImage();

      void handleInput();

      void parsePlugins();

    private:
      /*
       * This function is used to create the output file name thanks to the
       * input one.
       */
      void createPPMFile();

      /*
       * This function is used to write colors of the image in the output file
       */
      void writeColor(std::ofstream &file, sf::Color color);

      /*
       * This function is used to create the name of output file name thanks to
       * the input one.
       */
      void createOutputFileName();

      /*
       * This function is used to check if the input file has been updated
       * during the program to update scene 
      */
      void checkFileChange();

      /*
       * This attribut is used to keep the input file during the program.
       */
      std::string _inputFilePath;

      /*
       * This attribut is used to keep the output file during the program.
       */
      std::string _outputFilePath;

      std::filesystem::file_time_type _ftime;
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
      Raytracer::Renderer _renderer;
      Raytracer::Camera _camera;
      void changeCamQuality();
      bool _cameraMoved = false;
      const std::chrono::duration<float> _qualityUpdateDelay{0.5f};
      bool _userQuit = false;
  };
}  // namespace Raytracer
