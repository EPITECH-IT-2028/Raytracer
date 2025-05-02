#include <cstring>
#include "Renderer.hpp"
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
  Raytracer::Scene scene(800, 600, av[1]);
  scene.render();
  return 0;
}
