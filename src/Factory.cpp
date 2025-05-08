#include "Factory.hpp"
#include <filesystem>
#include <dlfcn.h>
#include <string>

void Raytracer::Factory::initFactories(const std::vector<std::string> &plugins) {
  for (const auto &plugin : plugins) {
    std::string pluginName = plugin.substr(plugin.find_last_of('/') + 1);
    pluginName = pluginName.substr(0, pluginName.find_last_of('.'));
    void *handle = dlopen(plugin.c_str(), RTLD_LAZY);
    if (!handle)
      continue;
    using AddShapeFunc = Raytracer::IShape *(*)();
    AddShapeFunc addShape = (AddShapeFunc)dlsym(handle, "addShape");
    if (addShape) {
      registerShape<Raytracer::IShape>(pluginName, addShape);
      continue;
    }
    using AddLightFunc = Raytracer::ILight *(*)();
    AddLightFunc addLight = (AddLightFunc)dlsym(handle, "addLight");
    if (addLight) {
      registerLight<Raytracer::ILight>(pluginName, addLight);
      continue;
    }
    dlclose(handle);
  }
}
