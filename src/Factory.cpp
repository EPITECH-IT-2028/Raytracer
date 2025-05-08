#include "Factory.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <string>

/**
 * @brief Initializes and registers plugin factories from a list of shared library paths.
 *
 * Attempts to dynamically load each plugin file and register its factory function for shapes, lights, or materials, based on the available exported symbols. If a plugin does not provide any recognized factory function, it is ignored. Throws a runtime error if a plugin cannot be loaded.
 *
 * @param plugins List of file paths to plugin shared libraries.
 */
void Raytracer::Factory::initFactories(
    const std::vector<std::string> &plugins) {
  for (const auto &plugin : plugins) {
    std::string pluginName = plugin.substr(plugin.find_last_of('/') + 1);
    pluginName = pluginName.substr(0, pluginName.find_last_of('.'));
    void *handle = dlopen(plugin.c_str(), RTLD_LAZY);
    if (!handle) {
      throw std::runtime_error("Failed to load plugin: " + plugin);
    }
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
    using AddMaterialFunc = Raytracer::IMaterials *(*)();
    AddMaterialFunc addMaterial = (AddMaterialFunc)dlsym(handle, "addMaterial");
    if (addMaterial) {
      registerMaterial<Raytracer::IMaterials>(pluginName, addMaterial);
      continue;
    }
    dlclose(handle);
  }
}
