#include <cstring>
#include "Renderer.hpp"
#include "Scene.hpp"

int main(void) {
  Raytracer::Scene scene(800, 600);
  scene.render();
  return 0;
}
