#include "Renderer.hpp"
#include "Scene.hpp"

int main(void) {
  Raytracer::Scene scene(400, 400);
  scene.render();
  return 0;
}
