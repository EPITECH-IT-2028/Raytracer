#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "LightComposite.hpp"
#include "Ray.hpp"
#include "ShapeComposite.hpp"

namespace Raytracer {
  /**
   * @brief Handles the rendering process of the 3D scene.
   *
   * This class is responsible for initializing the scene from a configuration file,
   * casting rays, calculating colors based on intersections and lighting,
   * and outputting the final image to a buffer or file.
   */
  class Renderer {
    public:
      /**
       * @brief Default constructor for Renderer.
       */
      Renderer() = default;
      /**
       * @brief Constructs a Renderer object and initializes the scene.
       * @param width The width of the rendering viewport.
       * @param height The height of the rendering viewport.
       * @param inputFilePath The path to the scene configuration file.
       * @param cam A reference to the camera object.
       * @param plugins A list of plugin file paths.
       */
      Renderer(int width, int height, const std::string &inputFilePath,
               Camera &cam, const std::vector<std::string> &plugins)
          : _width(width),
            _height(height),
            _inputFilePath(inputFilePath),
            _shapes(ShapeComposite()),
            _lights(LightComposite()),
            _plugins(plugins) {
        initScene(cam);
      }

      /**
       * @brief Writes the rendered scene to a file (not fully implemented).
       * @param filename The name of the file to write to.
       */
      void writeInFile(const std::string &filename);

      /**
       * @brief Initializes the scene by parsing the configuration file.
       * @param camera A reference to the camera to be configured.
       */
      void initScene(Camera &camera);

      /**
       * @brief Calculates the color of a ray after interacting with the scene.
       * @param r The ray to trace.
       * @param s The composite of shapes in the scene.
       * @param light The composite of lights in the scene.
       * @param cameraPos The camera object (used for view-dependent effects like specular highlights).
       * @param depth The current recursion depth for reflections/refractions.
       * @return Math::Vector3D The calculated color for the ray.
       */
      Math::Vector3D rayColor(Ray &r, const ShapeComposite &s,
                              const LightComposite &light,
                              const Camera &cameraPos, int depth);

      /**
       * @brief Gets the input file path for the scene configuration.
       * @return const std::string& A const reference to the input file path.
       */
      const std::string &getInputFilePath() const {
        return _inputFilePath;
      }

      /**
       * @brief Renders the scene to a framebuffer.
       * @param framebuffer A reference to the vector of SFML colors representing the framebuffer.
       * @param cam A reference to the camera used for rendering.
       * @param isHighQuality A boolean indicating whether to render in high quality.
       */
      void renderToBuffer(std::vector<sf::Color> &framebuffer,
                          Raytracer::Camera &cam, bool isHighQuality);

      /**
       * @brief Sets the width of the rendering viewport.
       * @param width The new width.
       */
      void setWidth(int width) {
        _width = width;
      }
      /**
       * @brief Sets the height of the rendering viewport.
       * @param height The new height.
       */
      void setHeight(int height) {
        _height = height;
      }

    private:
      int _width;
      int _height;
      std::string _inputFilePath;
      ShapeComposite _shapes; ///< Composite object holding all shapes in the scene.
      LightComposite _lights; ///< Composite object holding all lights in the scene.
      std::vector<std::string> _plugins; ///< List of plugin file paths.
      int _maxDepth = 5;      ///< Maximum recursion depth for ray tracing (e.g., for reflections).
  };
}  // namespace Raytracer
