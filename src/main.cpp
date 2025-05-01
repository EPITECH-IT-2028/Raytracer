#include <cstring>
#include "Renderer.hpp"
#include "Scene.hpp"

int main(const int ac, const char **av) {
  if (ac != 2 || !av) {
    std::cerr << "[ERROR] - You need only two arguments to start this project.\nUse -h flag to have more information.\n" << std::endl;
    return 84;
  }
  if (strcmp(av[1], "-h") == 0) {
    std::string helpMessage = "USAGE:\t./raytracer <SCENE_FILE>\n  SCENE_FILE: scene configuration (*.cfg)";
    std::cout << helpMessage << std::endl;
    return 0;
  }
  Raytracer::Renderer renderer;
  renderer.writeInFile("./scenes/example.cfg");
  std::cout << "Rendering complete. Check output.ppm for the result."
            << std::endl;
  Raytracer::Scene scene;
  scene.init();
  scene.render();
  return 0;
}
