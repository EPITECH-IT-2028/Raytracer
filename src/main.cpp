#include <cstring>
#include <iostream>
#include "Scene.hpp"

int main(int ac, char **av) {
  if (ac != 2 || !av) {
    std::cerr << "[ERROR] - You need only two arguments to start this project.\nUse -h flag to have more information.\n" << std::endl;
    return 84;
  }
  if (strcmp(av[1], "-h") == 0) {
    std::string helpMessage = "USAGE:\t./raytracer <SCENE_FILE>\n  SCENE_FILE: scene configuration (*.cfg)";
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
