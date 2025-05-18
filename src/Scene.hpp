#include <SFML/Graphics.hpp>
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>
#include "Camera.hpp"
#include "Renderer.hpp"

namespace Raytracer {
  /**
   * @brief Manages the overall scene, rendering, and user interaction.
   *
   * This class is responsible for initializing the scene from a configuration file,
   * handling user input for camera control, managing the rendering loop,
   * and displaying the rendered image. It also handles plugin loading.
   */
  class Scene {
    public:
      /**
       * @brief Constructor for the Scene class.
       * @param width The width of the scene/window.
       * @param height The height of the scene/window.
       * @param inputFilePath The path to the scene configuration file.
       */
      Scene(int width, int height, const std::string &inputFilePath);

      /**
       * @brief Destructor for the Scene class.
       */
      ~Scene();

      /**
       * @brief Renders the scene.
       */
      void render();

      /**
       * @brief Initializes the scene components.
       */
      void init();

      /**
       * @brief Updates the image buffer with the rendered scene.
       */
      void updateImage();

      /**
       * @brief Handles user input for camera movement and other interactions.
       */
      void handleInput();

      /**
       * @brief Parses and loads plugins.
       */
      void parsePlugins();

    private:
      /**
       * @brief Creates the output PPM file with the rendered image.
       */
      void createPPMFile();

      /**
       * @brief Writes a color to the output file stream.
       * @param file The output file stream.
       * @param color The color to write.
       */
      void writeColor(std::ofstream &file, sf::Color color);

      /**
       * @brief Creates the output file name based on the input file name.
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
      int _width; ///< Width of the scene/window.
      int _height; ///< Height of the scene/window.
      sf::RenderWindow _window; ///< SFML render window.
      sf::Texture _texture; ///< Texture for displaying the rendered image.
      sf::Sprite _sprite; ///< Sprite for displaying the texture.
      sf::Image _image; ///< Image buffer for rendering.
      std::vector<sf::Color> _framebuffer; ///< Framebuffer storing pixel colors.
      std::chrono::time_point<std::chrono::steady_clock> _lastMovement; ///< Timestamp of the last camera movement.
      bool _isHighQuality = true; ///< Flag indicating if rendering is in high quality.
      const float _updateQuality = 0.5f; ///< Time in seconds to update quality after movement.
      std::vector<std::string> _plugins; ///< List of plugin file paths.
      std::vector<void *> _pluginHandles; ///< Handles to loaded plugins.
      std::unique_ptr<Raytracer::Renderer> _renderer;
      Raytracer::Camera _camera; ///< The scene camera.

      std::string _inputFilePath; ///< Path to the input scene configuration file.
      std::string _outputFilePath; ///< Path to the output image file.

      /**
       * @brief Toggles the camera rendering quality between high and low.
       *
       * This method adjusts rendering parameters to switch between a faster,
       * lower-quality preview and a slower, higher-quality final render.
       * It typically gets called based on camera movement to provide interactive
       * feedback.
       */
      void changeCamQuality();
      bool _cameraMoved = false; ///< Flag indicating if the camera has moved recently.
      const std::chrono::duration<float> _qualityUpdateDelay{0.5f}; ///< Delay before updating to high quality after movement.
      bool _userQuit = false; ///< Flag indicating if the user has requested to quit.
  };
}  // namespace Raytracer
