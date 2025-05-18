#include <cstring>
#include <iostream>
#include "Scene.hpp"

/**
 * @brief The main entry point for the Raytracer application.
 *
 * This function handles command-line arguments, initializes the scene,
 * and starts the rendering process.
 * It expects one argument: the path to a scene configuration file (*.cfg).
 * The -h flag can be used to display usage information.
 *
 * @param ac The number of command-line arguments.
 * @param av An array of C-style strings representing the command-line arguments.
 *           av[0] is the program name, av[1] should be the scene file path or -h.
 * @return int Returns 0 on successful execution, 84 on error (e.g., incorrect arguments, file not found, rendering error).
 */
int main(int ac, char **av) {
  if (ac != 2 || !av) {
    std::cerr << "[ERROR] - You need only two arguments to start this "
                 "project.\nUse -h flag to have more information.\n"
              << std::endl;
    return 84;
  }
  if (strcmp(av[1], "-h") == 0) {
    std::string helpMessage =
        "USAGE:\t./raytracer <SCENE_FILE>\n  SCENE_FILE: scene configuration "
        "(*.cfg)";
    std::cout << helpMessage << std::endl;
    return 0;
  }

  try {
    Raytracer::Scene scene(800, 600, av[1]);
    scene.render();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 84;
  } catch (...) {
    std::cerr << "[ERROR] - Unknown error occurred." << std::endl;
    return 84;
  }
  return 0;
}
