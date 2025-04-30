#include "Renderer.hpp"
#include "Scene.hpp"

int main(void) {
  Raytracer::Renderer renderer(400, 400);
  renderer.writeInFile("output.ppm");
  std::cout << "Rendering complete. Check output.ppm for the result."
            << std::endl;
  Raytracer::Scene scene;
  scene.init();
  scene.render();
  return 0;
}
