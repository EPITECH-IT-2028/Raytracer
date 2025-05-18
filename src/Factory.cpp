#include "Factory.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <string>

/**
 * @brief Initializes the factories by loading and registering creators from plugins.
 *
 * This method iterates over a list of plugin file paths (shared libraries, e.g., .so files).
 * For each plugin, it attempts to load the library and find specific factory functions:
 *  - "addShape": For creating IShape objects.
 *  - "addLight": For creating ILight objects.
 *  - "addMaterial": For creating IMaterials objects.
 * If a factory function is found, it's registered with the corresponding factory map
 * using the plugin's filename (without extension) as the key.
 *
 * @param plugins A vector of strings, where each string is the path to a plugin file.
 */
void Raytracer::Factory::initFactories(
    const std::vector<std::string> &plugins) {
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
    using AddMaterialFunc = Raytracer::IMaterials *(*)();
    AddMaterialFunc addMaterial = (AddMaterialFunc)dlsym(handle, "addMaterial");
    if (addMaterial) {
      registerMaterial<Raytracer::IMaterials>(pluginName, addMaterial);
      continue;
    }
    dlclose(handle);
  }
}
